// -----------------------------------------------------------------------------------------
//  File          : NavMeshSurface.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/03
//  Updated       : 2025/08/06
//  Description   : ナビメッシュ管理とナビメッシュ経路探索クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NAV_MESH_SURFACE_H_
#define NAV_MESH_SURFACE_H_

#include <string>
#include <vector>
#include <gslib.h>
#include <utility> 

// エラーノード
const int ERROR_NODE_ID{ -1 };

class NavMeshSurface {
public:
	NavMeshSurface(const std::string& mesh_path);

	~NavMeshSurface();

public:
	// 経路探索グラフのノード
	struct NavNode {
		NavNode() : 
			id{ 0 },
			indices{ 0, 0, 0 }
		{}

		int id;						// ポリゴンのインデックス
		GLuint indices[3];			// 構成する頂点のインデックス
		GSvector3 center;			// ポリゴンの中心座標
        std::vector<int> neighbors_id;	// 隣接するポリゴンのIDリスト
	};

private:
	// 頂点データ構造体
	struct Vertex {
		GSvector3 position;
	};

	// GSvector3をmapのキーとして使えるようにする
	struct CompareGSvector3 {
		bool operator()(const GSvector3& a, const GSvector3& b) const {
			if (a.x != b.x) return a.x < b.x;
			if (a.y != b.y) return a.y < b.y;
			return a.z < b.z;
		}
	};

public:
	/// <summary>
	/// ナビメッシュを更新する
	/// </summary>
	void update(float delta_time);

	/// <summary>
	/// ナビメッシュを描画する
	/// </summary>
	void draw() const;

	/// <summary>
	/// ナビメッシュを読み込む
	/// </summary>
	/// <param name="mesh_path">= ファイルパス</param>
	void load(const std::string& mesh_path);

	/// <summary>
	/// 破棄
	/// </summary>
	void clear();

	/// <summary>
	/// 経路探索
	/// </summary>
	/// <param name="start">= 開始地点</param>
	/// <param name="end">= 終了地点</param>
	/// <returns>経路</returns>
	vector<int> find_path(const GSvector3& start, const GSvector3& end) const;

	/// <summary>
	/// 直線的な経路を生成
	/// </summary>
	/// <param name="path">= 経路探索結果</param>
	/// <param name="start">= 開始地点</param>
	/// <param name="end">= 終了地点</param>
	/// <returns>直線的にした経路</returns>
	vector<GSvector3> create_line_path(const std::vector<int>& path) const;

	/// <summary>
	/// 滑らかな経路を生成
	/// </summary>
	/// <param name="path">= 経路探索結果</param>
	/// <param name="start">= 開始地点</param>
	/// <param name="end">= 終了地点</param>
	/// <returns>滑らかにした経路</returns>
	vector<GSvector3> create_smooth_path(const std::vector<int>& path, const GSvector3& start, const GSvector3& end) const;

private:
	/// <summary>
	/// バッファオブジェクトの作成
	/// </summary>
	/// <param name="target">= </param>
	/// <param name="size">= </param>
	/// <param name="data">= </param>
	/// <returns>バッファオブジェクト番号</returns>
	GLuint create_buffer(GLenum target, GLuint size, const GLvoid* data);

	/// <summary>
	/// 頂点配列オブジェクトの作成
	/// </summary>
	/// <returns>頂点配列オブジェクト番号</returns>
	GLuint create_vertex_array();

	/// <summary>
	/// 経路探索グラフを生成する
	/// </summary>
	void build_graph(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices);

	/// <summary>
	/// ポリゴンの中心を求める
	/// </summary>
	/// <param name="v0">= 頂点</param>
	/// <param name="v1">= 頂点</param>
	/// <param name="v2">= 頂点</param>
	/// <returns>中心</returns>
	GSvector3 calc_polygon_center(const GSvector3& v0, const GSvector3& v1, const GSvector3& v2) const;

	/// <summary>
	/// 座標から最も近いポリゴンを検索
	/// </summary>
	/// <param name="position">= 座標</param>
	/// <returns>ポリゴン</returns>
	int find_nearest_polygon(const GSvector3& position) const;

private:
	// 描画する頂点数
	GLuint index_count_{ 0 };
	// インデックスバッファ
	GLuint indices_{ 0 };
	// 頂点バッファ
	GLuint vertices_{ 0 };
	// 頂点配列オブジェクト
	GLuint vertex_array_{ 0 };

	// 経路探索用グラフデータ
    std::vector<NavNode> graph_;
	// 滑らかな経路探索用データ
    std::vector<Vertex> path_vertices_;

#ifdef _DEBUG
	// ナビメッシュを描画するかどうか
	bool draw_navmesh_{ false };
#endif

public:
	// コピー禁止
	NavMeshSurface(const NavMeshSurface& other) = delete;
	NavMeshSurface& operator = (const NavMeshSurface& other) = delete;

};

#endif
