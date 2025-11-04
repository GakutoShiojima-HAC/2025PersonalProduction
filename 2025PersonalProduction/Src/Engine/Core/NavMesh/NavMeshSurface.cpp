#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <limits>
#include <unordered_map>

#ifdef _DEBUG
#include <imgui/imgui.h>
#include "Engine/Utils/DebugMarker.h"
#endif

NavMeshSurface::NavMeshSurface(const std::string& mesh_path) {
	load(mesh_path);
}

NavMeshSurface::~NavMeshSurface() {
	clear();
}

void NavMeshSurface::update(float delta_time) {
#ifdef _DEBUG
    ImGui::Begin("Field Window");

    ImGui::Separator();
    ImGui::Text("NavMesh Surface Option");

    if (ImGui::Button("Toggle Draw NavMesh")) {
        draw_navmesh_ = !draw_navmesh_;
    }
    if (ImGui::Button("Toggle Draw Pattern")) {
        draw_wireframe_ = !draw_wireframe_;
    }
    if (ImGui::Button("Toggle Funnel Path")) {
        enable_funnel_ = !enable_funnel_;
    }

    ImGui::End();
#endif
}

void NavMeshSurface::draw() const {
    if (vertices_.empty() || polygons_.empty()) return;

#ifdef _DEBUG
    if (!draw_navmesh_) return;
    // ライティングを無効化
    glDisable(GL_LIGHTING);
    // 色を設定
    glColor4f(0.0f, 1.0f, 1.0f, 0.5f);

    // 描画をワイヤーフレームか塗りつぶしどちらで行うか
    GLenum mode = draw_wireframe_ ? GL_LINES : GL_TRIANGLES;
    glBegin(mode);

    // 全てのポリゴンをループ
    for (const auto& poly : polygons_) {
        // 頂点座標を取得
        const GSvector3& v0 = vertices_[poly.vertex_indices[0]];
        const GSvector3& v1 = vertices_[poly.vertex_indices[1]];
        const GSvector3& v2 = vertices_[poly.vertex_indices[2]];
        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);

        if (draw_wireframe_) {
            glVertex3f(v1.x, v1.y, v1.z);
            glVertex3f(v2.x, v2.y, v2.z);

            glVertex3f(v2.x, v2.y, v2.z);
            glVertex3f(v0.x, v0.y, v0.z);
        }
        else {
            glVertex3f(v2.x, v2.y, v2.z);
        }
    }
    
    // 状態を戻す
    glEnd();
    glEnable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
#endif
}

void NavMeshSurface::clear() {
    vertices_.clear();
    polygons_.clear();

    // 頂点配列オブジェクトの削除
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    // 頂点バッファオブジェクトの削除
    if (vbo_vertices_ != 0) {
        glDeleteBuffers(1, &vbo_vertices_);
        vbo_vertices_ = 0;
    }
    // インデックスバッファオブジェクトの削除
    if (ibo_indices_ != 0) {
        glDeleteBuffers(1, &ibo_indices_);
        ibo_indices_ = 0;
    }
    index_count_ = 0;
}

bool NavMeshSurface::find(const GSvector3& start, const GSvector3& goal, std::vector<GSvector3>& out, float offset_ratio) const {
    out.clear();

    // スタートとゴールのポリゴンを検索
    int start_index = find_polygon(start);
    int goal_index = find_polygon(goal);
    // 範囲外なら終了
    if (start_index == -1 || goal_index == -1) {
        return false;
    }
    // スタートとゴールが同じポリゴンに存在する
    if (start_index == goal_index) {
        out.push_back(start);
        out.push_back(goal);
        return true;
    }

    // 障害物がなく真っ直ぐ進める
    if (raycast(start, goal, start_index)) {
        out.push_back(start);
        out.push_back(goal);
        return true;
    }

    // Astarでポリゴン探索
    std::vector<int> a_star_path;
    // 経路が見つからない
    if (!find_path_a_star(start_index, goal_index, goal, a_star_path)) {
        return false;
    }

#ifdef _DEBUG
        // Asterだけの結果
    if (!enable_funnel_) {
        out.push_back(start);
        for (const auto& pory : a_star_path) {
            out.push_back(polygons_[pory].center);
        }
        out.push_back(goal);
        return true;
    }
#endif

    // 滑らかにする
    create_path_with_funnel(a_star_path, start, goal, start_index, out, offset_ratio);
    return true;
}

void NavMeshSurface::load(const std::string& mesh_path) {
    // 既存のデータをクリア
    vertices_.clear();
    polygons_.clear();

    std::ifstream file(mesh_path);
    if (!file.is_open()) return;

    std::string line;
    // 読み込み中の行数
    int line_number = 0;

    // 頂点数を取得
    if (!std::getline(file, line)) return;
    line_number++;
    int vertex_count = std::stoi(line);

    // 頂点が存在しない
    if (vertex_count <= 0) return;
    // 頂点分メモリを確保
    vertices_.reserve(vertex_count);

    // 頂点を読み込む
    for (int i = 0; i < vertex_count; ++i) {
        // 行を読み進める
        if (!std::getline(file, line)) return;
        line_number++;

        std::stringstream ss(line);
        char prefix;
        GSvector3 v;
        ss >> prefix >> v.x >> v.y >> v.z;

        // 頂点データ扱いでなければエラー
        if (ss.fail() || prefix != 'v') return;
        vertices_.push_back(v);
    }

    // ポリゴン数を取得
    if (!std::getline(file, line)) return;
    line_number++;
    int polygon_count = std::stoi(line);

    // ポリゴンが存在しない
    if (polygon_count <= 0) return;
    // ポリゴン分メモリを確保
    polygons_.reserve(polygon_count);

    // ポリゴンと隣接リストを読み込む
    for (int i = 0; i < polygon_count; ++i) {
        NavMeshPolygon poly;

        // ポリゴンデータを取得
        if (!std::getline(file, line)) return;
        line_number++;
        std::stringstream ss_p(line);
        char prefix_p;
        ss_p >> prefix_p >> poly.vertex_indices[0] >> poly.vertex_indices[1] >> poly.vertex_indices[2];
        // ポリゴンデータ扱いでなければエラー
        if (ss_p.fail() || prefix_p != 'p') return;

        // 隣接リストデータを取得
        if (!std::getline(file, line)) return;
        line_number++;
        std::stringstream ss_n(line);
        char prefix_n;
        ss_n >> prefix_n >> poly.neighbor_indices[0] >> poly.neighbor_indices[1] >> poly.neighbor_indices[2];
        // 隣接リストデータ扱いでなければエラー
        if (ss_n.fail() || prefix_n != 'n') return;

        // ポリゴンの中心点を計算
        const auto& v0 = vertices_[poly.vertex_indices[0]];
        const auto& v1 = vertices_[poly.vertex_indices[1]];
        const auto& v2 = vertices_[poly.vertex_indices[2]];
        poly.center.x = (v0.x + v1.x + v2.x) / 3.0f;
        poly.center.y = (v0.y + v1.y + v2.y) / 3.0f;
        poly.center.z = (v0.z + v1.z + v2.z) / 3.0f;

        polygons_.push_back(poly);
    }

#ifdef _DEBUG
    // デバッグ用バッファを作成
    create_buffer();
#endif
}

void NavMeshSurface::create_buffer() {
    if (vertices_.empty() || polygons_.empty()) return;

    std::vector<GLuint> indices;
    // 頂点数分メモリを確保(ポリゴンが三角形前提)
    indices.reserve(polygons_.size() * 3);

    for (const auto& poly : polygons_) {
        // GL_TRIANGLES 用にインデックスを追加
        indices.push_back((GLuint)poly.vertex_indices[0]);
        indices.push_back((GLuint)poly.vertex_indices[1]);
        indices.push_back((GLuint)poly.vertex_indices[2]);
    }
    // 描画するインデックス数を保存
    index_count_ = (GLsizei)indices.size();

    // 頂点バッファの作成
    glGenBuffers(1, &vbo_vertices_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GSvector3) * vertices_.size(),
        vertices_.data(),
        GL_STATIC_DRAW);

    // インデックスバッファの作成
    glGenBuffers(1, &ibo_indices_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_indices_);
    // 作成したインデックスリスト(std::vector<GLuint>)からデータを転送
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(GLuint) * indices.size(),
        indices.data(),
        GL_STATIC_DRAW);

    // 頂点配列オブジェクトの作成と設定
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // 頂点配列オブジェクトに頂点バッファを記録
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);

    // 頂点属性の設定
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GSvector3), (void*)0);
    glEnableVertexAttribArray(0); // 属性0番を有効化

    // 頂点配列オブジェクトにインデックスバッファを記録
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_indices_);

    // バインド解除
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool NavMeshSurface::find_path_a_star(int start_index, int goal_index, const GSvector3& goal_pos, std::vector<int>& out) const {
    out.clear();

    // XZ平面上での距離
    auto distance_xz = [](const GSvector3& vec, const GSvector3& other) {
        float dx = vec.x - other.x;
        float dz = vec.z - other.z;
        return std::sqrt(dx * dx + dz * dz);
        };

    // 初期化
    const int num_polygons = (int)polygons_.size();
    constexpr float infinity = std::numeric_limits<float>::infinity();
    // オープンリスト(F)
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> open_set;
    // 親ノード(どのポリゴンから来たか)
    std::vector<int> from(num_polygons, -1);

    // スタートからの実コスト(G)
    std::vector<float> g_score(num_polygons, infinity);
    // ゴールまでの推定コスト(H)
    // (Funnel Algorithm導入時にはゴール「地点」までの距離に修正すると精度が上がる)
    std::vector<float> h_score(num_polygons);
    for (int i = 0; i < num_polygons; ++i) {
        h_score[i] = distance_xz(polygons_[i].center, goal_pos);
    }
    // スタート
    int current_index = -1; // 初期値はNULLポリゴン
    g_score[start_index] = 0;
    open_set.push({ start_index, h_score[start_index] });

    while (!open_set.empty()) {
        current_index = open_set.top().polygon_index;
        open_set.pop();

        // ゴールに到達
        if (current_index == goal_index) break;

        const auto& current_poly = polygons_[current_index];

        // 隣接ポリゴンをチェック
        for (int i = 0; i < 3; ++i) {
            int neighbor_index = current_poly.neighbor_indices[i];

            // 隣が存在しないか、既に処理済み
            if (neighbor_index == -1) continue;

            // 移動コスト(G)の計算
            const auto& neighbor_poly = polygons_[neighbor_index];
            float cost = distance_xz(current_poly.center, neighbor_poly.center);    // コスト = ポリゴン中心間の距離

            float tentative_g_score = g_score[current_index] + cost;

            // より良い経路が見つかったら
            if (tentative_g_score < g_score[neighbor_index]) {
                from[neighbor_index] = current_index; // 最短経路を更新
                g_score[neighbor_index] = tentative_g_score;

                // Fコストを計算してオープンリストに追加
                float f_score = tentative_g_score + h_score[neighbor_index];
                open_set.push({ neighbor_index, f_score });
            }
        }
    }

    // ゴールにたどり着けなかったら
    if (current_index != goal_index) return false;

    // 親ノードを辿る
    std::vector<int> polygon_path;
    int tmp_index = goal_index;
    while (tmp_index != -1) {
        out.push_back(tmp_index);
        tmp_index = from[tmp_index];
    }
    // 逆にする(ゴールから格納されているため)
    std::reverse(out.begin(), out.end());
    return true;
}

void NavMeshSurface::create_path_with_funnel(const std::vector<int>& polygon_path, const GSvector3& start, const GSvector3& goal, int start_index, std::vector<GSvector3>& out, float offset_ratio) const {
    // ポータルリストの作成
    std::vector<NavMeshPortal> portals;
    portals.reserve(polygon_path.size());

    // Astarのポリゴン経路からポータルを作成
    for (size_t i = 0; i < polygon_path.size() - 1; ++i) {
        portals.push_back(get_portal(polygon_path[i], polygon_path[i + 1]));
    }
    // 最後のポータルとしてゴールを追加
    portals.push_back({ goal, goal });

    // 最初はスタート
    out.push_back(start);

    GSvector3 apex = start;
    int apex_poly_index = start_index;
    GSvector3 funnel_left = start;
    GSvector3 funnel_right = start;

    // どのポータルから来たか
    int left_portal_index = 0;
    int right_portal_index = 0;

    // AとBのXZ座標が同一かチェックするラムダ式
    auto is_xz_equal = [](const GSvector3& a, const GSvector3& b) {
        // (安全のため、直接比較ではなく、わずかな誤差(epsilon)を許容するのが望ましい)
        const float EPSILON = 0.001f;//1e-6f;
        return std::abs(a.x - b.x) < EPSILON && std::abs(a.z - b.z) < EPSILON;
        //return a.x == b.x && a.z == b.z;
        };

    // XZ平面上のみ線形補間するラムダ式
    auto lerp_xz = [](const GSvector3& a, const GSvector3& b, float t) {
        return GSvector3{
            a.x + (b.x - a.x) * t,
            a.y, // Y座標は元の高さを維持する
            a.z + (b.z - a.z) * t
        };
        };

    for (int i = 0; i < portals.size(); ++i) {
        const auto& portal = portals[i];

        GSvector3 portal_left_v = portal.v1;
        GSvector3 portal_right_v = portal.v2;

        // ポータルの頂点が、apexから見て常に左右になるようスワップ
        if (cross_product_xz(apex, portal_left_v, portal_right_v) < 0.0f) {
            std::swap(portal_left_v, portal_right_v);
        }

        // 右側
        // 新しい右ポータルが現在の右ファンネルより右かどうか(外側に展開)
        if (cross_product_xz(apex, funnel_right, portal_right_v) <= 0.0f) {
            // 新しい右ポータルが現在の左ファンネルより左かどうか(内側にあるか)
            if (is_xz_equal(apex, funnel_left) || cross_product_xz(apex, funnel_left, portal_right_v) <= 0.0f) {
                // 挟まったので更新
                funnel_right = portal_right_v;
                right_portal_index = i;
            }
            // 右ポータルが左ファンネルを超えて頂点が角になったら
            else {
                apex = funnel_left;
                apex_poly_index = polygon_path[left_portal_index];

                // 壁からどれだけ離すか
                if (offset_ratio > 0.0f && left_portal_index < portals.size()) {
                    const auto& corner_portal = portals[left_portal_index];
                    const GSvector3& other_vertex = is_xz_equal(apex, corner_portal.v1)
                        ? corner_portal.v2
                        : corner_portal.v1;
                    // apexを反対側の頂点に向かって割合分だけ移動させる
                    apex = lerp_xz(apex, other_vertex, offset_ratio);
                }

                // 直線で進めるかどうか
                if (raycast(apex, goal, apex_poly_index)) {
                    out.push_back(apex); // この角を追加
                    out.push_back(goal); // ゴールを追加して終了
                    return;
                }

                out.push_back(apex);

                // 左ファンネルの頂点からファンネルを再スタート
                funnel_left = apex;
                funnel_right = apex;
                i = left_portal_index;  // 巻き戻す
                left_portal_index = i;
                right_portal_index = i;
                continue;   // 再度探索
            }
        }
        // 新しい右ポータルが現在の右ファンネルより左かどうか(挟まっているか)
        else if (cross_product_xz(apex, funnel_right, portal_right_v) > 0.0f) {
            // 無条件で更新
            funnel_right = portal_right_v;
            right_portal_index = i;
        }

        // 左側
        // 新しい左ポータルが現在の左ファンネルより左かどうか(外側に展開)
        if (cross_product_xz(apex, funnel_left, portal_left_v) >= 0.0f) {
            // 新しい左ポータルが現在の右ファンネルより右かどうか(内側にあるか)
            if (is_xz_equal(apex, funnel_right) || cross_product_xz(apex, funnel_right, portal_left_v) >= 0.0f) {
                // 挟まったので更新
                funnel_left = portal_left_v;
                left_portal_index = i;
            }
            // 左ポータルが右ファンネルを超えて頂点が角になったら
            else {
                apex = funnel_right;
                apex_poly_index = polygon_path[right_portal_index];

                // 壁からどれだけ離すか
                if (offset_ratio > 0.0f && right_portal_index < portals.size()) {
                    const auto& corner_portal = portals[right_portal_index];
                    const GSvector3& other_vertex = is_xz_equal(apex, corner_portal.v1)
                        ? corner_portal.v2
                        : corner_portal.v1;
                    // apexを反対側の頂点に向かって割合分だけ移動させる
                    apex = lerp_xz(apex, other_vertex, offset_ratio);
                }

                // 直線で進めるかどうか
                if (raycast(apex, goal, apex_poly_index)) {
                    out.push_back(apex); // この角を追加
                    out.push_back(goal); // ゴールを追加して終了
                    return;
                }

                out.push_back(apex);

                // 右ファンネルの頂点からファンネルを再スタート
                funnel_left = apex;
                funnel_right = apex;
                i = right_portal_index; // 巻き戻す
                left_portal_index = i;
                right_portal_index = i;
                continue;   // 再度探索
            }
        }
        // 新しい左ポータルが現在の左ファンネルより右かどうか(挟まっているか)
        else if (cross_product_xz(apex, funnel_left, portal_left_v) < 0.0f) {
            // 無条件で更新
            funnel_left = portal_left_v;
            left_portal_index = i;
        }
    }
    // 最後にゴールを追加
    out.push_back(goal);
}

bool NavMeshSurface::is_in_polygon_xz(const GSvector3& position, const NavMeshPolygon& poly) const {
    // ポリゴンの頂点を取得
    const GSvector3& v0 = vertices_[poly.vertex_indices[0]];
    const GSvector3& v1 = vertices_[poly.vertex_indices[1]];
    const GSvector3& v2 = vertices_[poly.vertex_indices[2]];

    // 座標が三角形ABCの内部にあるか(外積を使った2D判定)
    float c1 = cross_product_xz(v0, v1, position);
    float c2 = cross_product_xz(v1, v2, position);
    float c3 = cross_product_xz(v2, v0, position);

    // 辺 AB, BC, CA に対して、点Pがすべて同じ(左か右)にあれば内部とする
    return (c1 >= 0.0f && c2 >= 0.0f && c3 >= 0.0f) ||
        (c1 <= 0.0f && c2 <= 0.0f && c3 <= 0.0f);
}

int NavMeshSurface::find_polygon(const GSvector3& position, float y_tolerance) const {
    for (size_t i = 0; i < polygons_.size(); ++i) {
        const auto& poly = polygons_[i];
        // ポリゴンの縦軸(Y)が許容誤差範囲内かどうかチェックする
        {
            const GSvector3& v0 = vertices_[poly.vertex_indices[0]];
            const GSvector3& v1 = vertices_[poly.vertex_indices[1]];
            const GSvector3& v2 = vertices_[poly.vertex_indices[2]];
            // ポリゴンのY座標の最小・最大を計算
            float min_y = std::min({ v0.y, v1.y, v2.y });
            float max_y = std::max({ v0.y, v1.y, v2.y });

            // 縦軸(Y)が許容誤差範囲内か
            if (position.y < (min_y - y_tolerance) || position.y >(max_y + y_tolerance)) continue;
        }
        // ポリゴンの範囲内か
        if (is_in_polygon_xz(position, poly)) return (int)i;
    }
    return -1;
}

float NavMeshSurface::cross_product_xz(const GSvector3& v1, const GSvector3& v2) const {
    return v1.x * v2.z - v1.z * v2.x;;
}

float NavMeshSurface::cross_product_xz(const GSvector3& a, const GSvector3& b, const GSvector3& p) const {
    return (b.x - a.x) * (p.z - a.z) - (b.z - a.z) * (p.x - a.x);
}

NavMeshPortal NavMeshSurface::get_portal(int poly_a_index, int poly_b_index) const {
    const auto& poly_a = polygons_[poly_a_index];

    // ポリゴンAのどの辺がポリゴンBに繋がっているか探す
    for (int i = 0; i < 3; ++i) {
        if (poly_a.neighbor_indices[i] == poly_b_index) {
            // i辺がポータル
            int v_index1 = poly_a.vertex_indices[i];
            int v_index2 = poly_a.vertex_indices[(i + 1) % 3]; // 次の頂点
            return { vertices_[v_index1], vertices_[v_index2] };
        }
    }
    // 到達しないが念のため
    return NavMeshPortal();
}

bool NavMeshSurface::raycast(const GSvector3& start, const GSvector3& goal, int start_index) const {
    int current_poly_index = start_index;
    GSvector3 current_ray_start = start;
    int prev_poly_index = -1;   // どこから来たか

    // 無限ループしないよう30でセーブ
    for (int i = 0; i < 30; ++i) {
        const auto& poly = polygons_[current_poly_index];
        // どの辺と交差するか
        int crossing_neighbor_index = -1;

        // ポリゴンの3辺をチェック
        for (int j = 0; j < 3; ++j) {
            const auto& v1 = vertices_[poly.vertex_indices[j]];
            const auto& v2 = vertices_[poly.vertex_indices[(j + 1) % 3]];

            // スタートからゴールへのレイと、辺が交差しているか
            if (intersect_segments_xz(start, goal, v1, v2)) {
                // 交差した辺の隣接ポリゴンを取得
                int neighbor = poly.neighbor_indices[j];
                // 来たポリゴンなら無視
                if (neighbor == prev_poly_index) continue;
                // 障害物に衝突
                if (neighbor == -1) return false;
                // 次に進むべきポリゴンが見つかったので辺チェックを終了
                crossing_neighbor_index = neighbor;
                break;
            }
        }

        if (crossing_neighbor_index != -1) {
            // 次のポリゴンに進む
            prev_poly_index = current_poly_index;
            current_poly_index = crossing_neighbor_index;
            // レイがゴールのあるポリゴンに到達したら終了
            if (is_in_polygon_xz(goal, polygons_[current_poly_index])) return true; 
            // 続けて探索
        }
        else {
            // どの辺とも交差しない
            return false;
        }
    }
    // 探索回数が多すぎる
    return false;
}

bool NavMeshSurface::intersect_segments_xz(const GSvector3& seg1_start, const GSvector3& seg1_end, const GSvector3& seg2_start, const GSvector3& seg2_end) const {
    // 線分1を基準に線分2の両端が左右にあるか
    float c1 = cross_product_xz(seg1_start, seg1_end, seg2_start);
    float c2 = cross_product_xz(seg1_start, seg1_end, seg2_end);
    // 線分2を基準に線分1の両端が左右にあるか
    float c3 = cross_product_xz(seg2_start, seg2_end, seg1_start);
    float c4 = cross_product_xz(seg2_start, seg2_end, seg1_end);

    return ((c1 >= 0.0f && c2 <= 0.0f) || (c1 <= 0.0f && c2 >= 0.0f)) && ((c3 >= 0.0f && c4 <= 0.0f) || (c3 <= 0.0f && c4 >= 0.0f));
}
