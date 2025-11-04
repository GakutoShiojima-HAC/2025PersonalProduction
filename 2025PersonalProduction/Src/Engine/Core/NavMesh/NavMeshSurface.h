// -----------------------------------------------------------------------------------------
//  File          : NavMeshSurface.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/03
//  Updated       : 2025/10/30
//  Description   : ナビメッシュ管理とナビメッシュ経路探索クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NAV_MESH_SURFACE_H_
#define NAV_MESH_SURFACE_H_

#include <string>
#include <vector>
#include <gslib.h>

struct NavMeshPolygon {
    // ポリゴンを構成する3つの頂点インデックス
    int vertex_indices[3];

    // 3つの辺に隣接するポリゴンのインデックス(-1で壁)
    // 0は辺0 - 1の隣
    // 1は辺1 - 2の隣
    // 2は辺2 - 0の隣
    int neighbor_indices[3];

    // ポリゴンの中心
    GSvector3 center;
};

struct AStarNode {
    // ポリゴンのインデックス
    int polygon_index;
    // Fコスト (Gコスト + Hコスト)
    float f_cost;

    // 優先度付きキューをFコストの最小ヒープとして機能させるため
    bool operator > (const AStarNode& other) const {
        return f_cost > other.f_cost;
    }
};

struct NavMeshPortal {
    // 2つのポリゴンの共有辺
    GSvector3 v1;
    GSvector3 v2;
};

class NavMeshSurface {
public:
	NavMeshSurface(const std::string& mesh_path);

	~NavMeshSurface();

public:
	void update(float delta_time);

	void draw() const;

	void clear();

public:
    /// <summary>
    /// 指定した2点間の経路を探索
    /// </summary>
    /// <param name="start">= スタート座標</param>
    /// <param name="goal">= ゴール座標</param>
    /// <param name="out">= 経路探索結果</param>
    /// <param name="offset_ratio">= 壁からどれだけ離すか 0.0~1.0で指定</param>
    /// <returns>探索に成功したら真を返却</returns>
    bool find(const GSvector3& start, const GSvector3& goal, std::vector<GSvector3>& out, float offset_ratio) const;

private:
    /// <summary>
    /// ナビメッシュを読み込む
    /// </summary>
    /// <param name="mesh_path">= ファイルパス</param>
    void load(const std::string& mesh_path);

    /// <summary>
    /// 描画用バッファの作成
    /// </summary>
    void create_buffer();

private:
    /// <summary>
    /// Astarアルゴリズムを実行し、ポリゴンでの経路を探索
    /// </summary>
    /// <param name="start_index">= スタートポリゴン</param>
    /// <param name="goal_index">= ゴールポリゴン</param>
    /// <param name="goal_pos">= ゴール座標</param>
    /// <param name="out"> = 結果</param>
    /// <returns>経路が見つかったら真を返却</returns>
    bool find_path_a_star(int start_index, int goal_index, const GSvector3& goal_pos, std::vector<int>& out) const;

    /// <summary>
    /// Funnelアルゴリズムを実行し、滑らかな経路を探索
    /// </summary>
    /// <param name="polygon_path">= ポリゴン上での経路</param>
    /// <param name="start">= スタート座標</param>
    /// <param name="goal">= ゴール座標</param>
    /// <param name="start_index">= スタートポリゴン</param>
    /// <param name="offset_ratio">= 壁からどれだけ離すか 0.0~1.0で指定</param>
    /// <param name="out">= 結果</param>
    void create_path_with_funnel(const std::vector<int>& polygon_path, const GSvector3& start, const GSvector3& goal, int start_index, std::vector<GSvector3>& out, float offset_ratio) const;

private:
    /// <summary>
    /// 指定した座標が、XZ平面上でポリゴンの内部に存在するかどうか
    /// </summary>
    /// <param name="position">= 座標</param>
    /// <param name="poly">= 対象のポリゴン</param>
    /// <returns>存在すれば真を返却</returns>
    bool is_in_polygon_xz(const GSvector3& position, const NavMeshPolygon& poly) const;

    /// <summary>
    /// 指定した座標から一番近いポリゴンを検索
    /// </summary>
    /// <param name="position">= 座標</param>
    /// <param name="y_tolerance">= 縦軸(Y)の許容誤差</param>
    /// <returns>見つかったらポリゴンのインデックスを、見つからなかったら-1</returns>
    int find_polygon(const GSvector3& position, float y_tolerance = 5.0f) const;

    /// <summary>
    /// 2Dベクトルの外積(のY成分)を計算(XZ平面上)
    /// </summary>
    /// <param name="v1">= ベクトル1</param>
    /// <param name="v2">= ベクトル2</param>
    /// <returns>外積のY成分</returns>
    float cross_product_xz(const GSvector3& v1, const GSvector3& v2) const;

    /// <summary>
    /// 2Dベクトルの外積(のY成分)を計算(XZ平面上)
    /// </summary>
    /// <param name="a"></param>
    /// <param name="b"></param>
    /// <param name="p"></param>
    /// <returns>外積のY成分</returns>
    float cross_product_xz(const GSvector3& a, const GSvector3& b, const GSvector3& p) const;

    /// <summary>
    /// 2つのポリゴンの共有辺を取得
    /// </summary>
    /// <param name="poly_a_index">= ポリゴンA</param>
    /// <param name="poly_b_index">= ポリゴンB</param>
    /// <returns>共有辺</returns>
    NavMeshPortal get_portal(int poly_a_index, int poly_b_index) const;

    /// <summary>
    /// 2点がNavMesh上で直線で結べるかどうか
    /// </summary>
    /// <param name="start">= スタート座標</param>
    /// <param name="goal">= ゴール座標</param>
    /// <param name="start_index">= スタートポリゴン/param>
    /// <returns>結べるなら真を返却</returns>
    bool raycast(const GSvector3& start, const GSvector3& goal, int start_index) const;

    /// <summary>
    /// 2つの線分(XZ平面)が交差しているか
    /// </summary>
    /// <param name="seg1_start">= 線分1のはじまり</param>
    /// <param name="seg1_end">= 線分1のおわり</param>
    /// <param name="seg2_start">= 線分2のはじまり</param>
    /// <param name="seg2_end">= 線分2のおわり</param>
    /// <returns>交差していれば真を返却</returns>
    bool intersect_segments_xz(const GSvector3& seg1_start, const GSvector3& seg1_end, const GSvector3& seg2_start, const GSvector3& seg2_end) const;

private:
    // 頂点リスト
    std::vector<GSvector3> vertices_;
    // 全てのポリゴンのリスト
    std::vector<NavMeshPolygon> polygons_;

    // 頂点配列オブジェクト
    GLuint vao_ = 0;
    // 頂点バッファ 
    GLuint vbo_vertices_ = 0;
    // インデックスバッファ
    GLuint ibo_indices_ = 0;
    // 描画するインデックスの数
    GLsizei index_count_ = 0;

#ifdef _DEBUG
	// ナビメッシュを描画するかどうか
	bool draw_navmesh_{ false };
    // ワイヤーフレームで描画するかどうか
    bool draw_wireframe_{ true };
    // Funnelアルゴリズムを適用するかどうか
    bool enable_funnel_{ true };
#endif

public:
	// コピー禁止
	NavMeshSurface(const NavMeshSurface& other) = delete;
	NavMeshSurface& operator = (const NavMeshSurface& other) = delete;

};

#endif
