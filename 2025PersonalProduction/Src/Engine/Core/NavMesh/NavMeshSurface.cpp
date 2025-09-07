#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility> 
#include <algorithm>
#include <queue>
#include <unordered_map> 
#include <cmath>

#ifdef _DEBUG
#include <imgui/imgui.h>
#include "Engine/Utils/DebugMarker.h"
#endif

NavMeshSurface::NavMeshSurface(const string& mesh_path) {
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

    if (ImGui::Button("Toggle Draw NavMesh")) draw_navmesh_ = !draw_navmesh_;

    ImGui::End();
#endif
}

void NavMeshSurface::draw() const {
#ifdef _DEBUG
    if (draw_navmesh_) {
        // ライティングを無効にする
        glDisable(GL_LIGHTING);
        glColor4f(0.0f, 1.0f, 1.0f, 0.2f);
        // 頂点配列オブジェクトのバインド
        glBindVertexArray(vertex_array_);
        // ポリゴンの描画
        glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_INT, nullptr);
        // 頂点配列オブジェクトのバインド解除
        glBindVertexArray(0);
        // ライティングを有効にする
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_LIGHTING);
    }
#endif
}

void NavMeshSurface::load(const string& mesh_path) {
    // メッシュデータの読み込み
    ifstream file{ mesh_path };
    // ファイルオープンに成功したか？
    if (!file) return;
    
    clear();

    vector<Vertex> vertices;
    vector<GLuint> indices;
    string line;

    // 頂点
    int vertex_count = 0;
    getline(file, line);
    vertex_count = stoi(line);
    vertices.reserve(vertex_count);
    for (int i = 0; i < vertex_count; ++i) {
        getline(file, line);
        stringstream ss(line);
        string value;
        Vertex v;
        getline(ss, value, ','); v.position.x = stof(value);
        getline(ss, value, ','); v.position.y = stof(value);
        getline(ss, value, ','); v.position.z = stof(value);
        vertices.push_back(v);
    }

    // インデックス
    int index_count = 0;
    getline(file, line);
    index_count = stoi(line);
    indices.reserve(index_count);
    for (int i = 0; i < index_count; i += 3) {
        getline(file, line);
        stringstream ss(line);
        string value;
        GLuint i1, i2, i3;
        getline(ss, value, ','); i1 = stoul(value);
        getline(ss, value, ','); i2 = stoul(value);
        getline(ss, value, ','); i3 = stoul(value);
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    vector<Vertex> final_vertices;
    vector<GLuint> final_indices;

    // 頂点マージ処理を呼び出す
    merging_duplicate_vertices(vertices, indices, final_vertices, final_indices);

    // 頂点数を取得
    index_count_ = (GLsizei)final_indices.size();
    // データを保持
    path_vertices_ = final_vertices;
    // 頂点バッファの作成
    vertices_ = create_buffer(GL_ARRAY_BUFFER, sizeof(Vertex) * final_vertices.size(), final_vertices.data());
    // インデックスバッファの作成
    indices_ = create_buffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * final_indices.size(), final_indices.data());
    // 頂点配列オブジェクトの作成
    vertex_array_ = create_vertex_array();
    // グラフを作成
    build_graph(final_vertices, final_indices);
}

void NavMeshSurface::clear() {
    // 頂点配列オブジェクトの削除
    glDeleteVertexArrays(1, &vertex_array_);
    // 頂点バッファオブジェクトの削除
    glDeleteBuffers(1, &vertices_);
    // インデックスバッファオブジェクトの削除
    glDeleteBuffers(1, &indices_);

    index_count_ = 0;
    indices_ = 0;
    vertices_ = 0;
    vertex_array_ = 0;

    graph_.clear();
    path_vertices_.clear();
}

vector<int> NavMeshSurface::find_path(const GSvector3& start, const GSvector3& end) const {
    // ナビメッシュが無い
    if (graph_.empty()) return vector<int>{};

    // スタートとゴールのポリゴンを検索
    int start_id = find_nearest_polygon(start);
    int end_id = find_nearest_polygon(end);
    if (start_id == ERROR_NODE_ID || end_id == ERROR_NODE_ID) return vector<int>{};

    // Astarデータ構造
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> open_list;
    unordered_map<int, bool> closed_list; // 評価済みノード
    unordered_map<int, int> parent_umap;
    unordered_map<int, float> goal_cost;

    auto heuristic = [&](int from_id, int to_id) {
        const auto& p1 = graph_[from_id].center;
        const auto& p2 = graph_[to_id].center;
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    };

    // 初期化
    for (const auto& poly : graph_) {
        goal_cost[poly.id] = numeric_limits<float>::max();
        closed_list[poly.id] = false;
    }
    goal_cost[start_id] = 0;
    parent_umap[start_id] = ERROR_NODE_ID;
    open_list.push({ heuristic(start_id, end_id), start_id });

    // Astar探索
    while (!open_list.empty()) {
        int current_id = open_list.top().second;
        open_list.pop();

        // 探索済みはスキップ
        if (closed_list[current_id]) continue;
        closed_list[current_id] = true;

        // ゴール到着
        if (current_id == end_id) {
            vector<int> path;
            while (current_id != ERROR_NODE_ID) {
                path.push_back(current_id);
                current_id = parent_umap[current_id];
            }
            reverse(path.begin(), path.end());
            
            // 探索終了
            return path;    
        }

        // 隣接ノードを探索
        for (int neighbor_id : graph_[current_id].neighbors_id) {
            // 探索済みはスキップ
            if (closed_list[neighbor_id]) continue;

            float tentative_goal_cost = goal_cost[current_id] + heuristic(current_id, neighbor_id); // コスト = 距離
            if (tentative_goal_cost < goal_cost[neighbor_id]) {
                parent_umap[neighbor_id] = current_id;
                goal_cost[neighbor_id] = tentative_goal_cost;
                float f_cost = tentative_goal_cost + heuristic(neighbor_id, end_id);
                open_list.push({ f_cost, neighbor_id });
            }
        }
    }

    // 探索失敗
    return vector<int>{};
}

vector<GSvector3> NavMeshSurface::create_line_path(const vector<int>& path) const {
    if (path.empty()) return vector<GSvector3>{};

    vector<GSvector3> final_path;
    for (int poly_id : path) {
        final_path.push_back(graph_[poly_id].center);
    }
    return final_path;
}

vector<GSvector3> NavMeshSurface::create_smooth_path(const vector<int>& path, const GSvector3& start, const GSvector3& end) const {
    // ノードが同じなら単純な直線移動で返却
    if (path.size() <= 1) return { start, end };

    vector<GSvector3> final_path;

    // 最初のウェイポイントとしてスタート地点を追加
    final_path.push_back(start);

    // TODO 滑らかな経路に変換する処理
    vector<GSvector3> tmp = create_line_path(path);
    final_path.insert(final_path.end(), tmp.begin(), tmp.end());

    // 最後のウェイポイントとしてゴール地点を追加
    final_path.push_back(end);

    return final_path;
}

GLuint NavMeshSurface::create_buffer(GLenum target, GLuint size, const GLvoid* data) {
    if (size == 0) return 0;
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, size, data, GL_STATIC_DRAW);
    glBindBuffer(target, 0);
    return buffer;
}

GLuint NavMeshSurface::create_vertex_array() {
    // 頂点配列オブジェクトの作成
    GLuint vertex_array = 0;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    // VBOと頂点属性の設定
    glBindBuffer(GL_ARRAY_BUFFER, vertices_);
    glEnableVertexAttribArray(0); // layout (location = 0) in vec3 aPos;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // EBOをVAOに紐付け
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_);

    // バインドを解除
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vertex_array;
}

void NavMeshSurface::build_graph(const vector<Vertex>& vertices, const vector<GLuint>& indices) {
    graph_.clear();

    int polygon_count = indices.size() / 3; // 三角形前提
    graph_.reserve(polygon_count);

    // ノードを作成
    for (int i = 0; i < polygon_count; ++i) {
        NavNode poly;
        poly.id = i;
        poly.indices[0] = indices[i * 3 + 0];
        poly.indices[1] = indices[i * 3 + 1];
        poly.indices[2] = indices[i * 3 + 2];

        // ポリゴンの中心点を計算
        const GSvector3& v0 = vertices[poly.indices[0]].position;
        const GSvector3& v1 = vertices[poly.indices[1]].position;
        const GSvector3& v2 = vertices[poly.indices[2]].position;
        poly.center = calc_polygon_center(v0, v1, v2);

        graph_.push_back(poly);
    }

    // キー(辺用の頂点) 値(辺を共有するノードのID)
    map<pair<GLuint, GLuint>, vector<int>> edge;

    // 辺の隣接計算
    for (int i = 0; i < polygon_count; ++i) {
        const NavNode& poly = graph_[i];
        GLuint p_indices[] = { poly.indices[0], poly.indices[1], poly.indices[2] };

        // ポリゴンの3つの辺をマップに登録
        for (int j = 0; j < 3; ++j) {
            GLuint v1 = p_indices[j];
            GLuint v2 = p_indices[(j + 1) % 3];
            // 辺の頂点インデックスをソートして、(1,2)と(2,1)が同じキーになるようにする
            pair<GLuint, GLuint> e = { min(v1, v2), max(v1, v2) };
            edge[e].push_back(i);
        }
    }

    for (const auto& pair : edge) {
        const vector<int>& polygons = pair.second;
        // 1つの辺を2つのポリゴンが共有している場合、それらは隣接している
        if (polygons.size() == 2) {
            int poly_id1 = polygons[0];
            int poly_id2 = polygons[1];
            graph_[poly_id1].neighbors_id.push_back(poly_id2);
            graph_[poly_id2].neighbors_id.push_back(poly_id1);
        }
    }
}

GSvector3 NavMeshSurface::calc_polygon_center(const GSvector3& v0, const GSvector3& v1, const GSvector3& v2) const {
    GSvector3 center;
    center.x = (v0.x + v1.x + v2.x) / 3.0f;
    center.y = (v0.y + v1.y + v2.y) / 3.0f;
    center.z = (v0.z + v1.z + v2.z) / 3.0f;
    return center;
}

int NavMeshSurface::find_nearest_polygon(const GSvector3& position) const {
    int id = ERROR_NODE_ID;
    float min_distance_sq = numeric_limits<float>::max();

    for (const auto& poly : graph_) {
        float dx = poly.center.x - position.x;
        float dy = poly.center.y - position.y;
        float dz = poly.center.z - position.z;
        float distance_sq = dx * dx + dy * dy + dz * dz;

        if (distance_sq < min_distance_sq) {
            min_distance_sq = distance_sq;
            id = poly.id;
        }
    }
    return id;
}

void NavMeshSurface::merging_duplicate_vertices(const vector<Vertex>& original_vertices, const vector<GLuint>& original_indices, vector<Vertex>& out_vertices, vector<GLuint>& out_indices) {
    // キー: 頂点座標, 値: 新しいユニークな頂点インデックス
    map<GSvector3, GLuint, CompareGSvector3> unique_vertices_map;

    out_vertices.clear();
    out_indices.clear();
    out_indices.reserve(original_indices.size());

    for (const GLuint original_index : original_indices) {
        const GSvector3& pos = original_vertices[original_index].position;

        // この座標が初めて見つかったかどうかをチェック
        if (unique_vertices_map.find(pos) == unique_vertices_map.end()) {
            // 初めて見つかった場合、新しいユニークな頂点としてリストに追加
            out_vertices.push_back({ pos });
            // 新しいインデックスを取得(現在のリストのサイズ - 1)
            GLuint new_index = (GLuint)(out_vertices.size() - 1);
            // マップに記録
            unique_vertices_map[pos] = new_index;
            // 追加
            out_indices.push_back(new_index);
        }
        else {
            // すでに見つかっているなら、マップから新しいインデックスを取得してリストに追加
            out_indices.push_back(unique_vertices_map[pos]);
        }
    }
}
