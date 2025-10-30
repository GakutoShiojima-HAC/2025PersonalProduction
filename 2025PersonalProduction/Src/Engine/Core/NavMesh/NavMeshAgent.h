// -----------------------------------------------------------------------------------------
//  File          : NavMeshAgent.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/05
//  Updated       : 2025/10/30
//  Description   : アクターをナビメッシュに沿って移動させる機能クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NAV_MESH_AGENT_H_
#define NAV_MESH_AGENT_H_

#include <vector>
#include <gslib.h>

using namespace std;

class NavMeshSurface;
class Actor;

class NavMeshAgent {
public:
	NavMeshAgent(Actor* target = nullptr, NavMeshSurface* navmesh = nullptr);

	~NavMeshAgent();

public:
	/// <summary>
	/// 移動対象を経路に沿って移動させる
	/// </summary>
	/// <param name="move_speed">= 移動速度</param>
	/// <param name="rotate_angle">= 方向転換時の回転量</param>
	void update_move(float delta_time, float move_speed, float rotate_angle);
	
	/// <summary>
	/// 経路を描画
	/// </summary>
	void draw_path() const;

	/// <summary>
	/// 経路探索(座標から)
	/// </summary>
	/// <param name="end">= 終了地点</param>
	/// <returns>探索に成功したら真を返却</returns>
	bool find_path(const GSvector3& end);

    /// <summary>
    /// 経路探索(アクターから)
    /// </summary>
    /// <param name="goal">= ゴールとするアクター</param>
    /// <returns>探索に成功したら真を返却</returns>
    bool find_path(Actor* goal);

public:
	/// <summary>
	/// 経路が見つかっているか
	/// </summary>
	/// <returns>見つかっていたら真を返却</returns>
	bool found_path() const;

	/// <summary>
	/// 移動が終了しているかどうか
	/// </summary>
	/// <returns>終了していたら真を返却</returns>
	bool is_end_move() const;

	/// <summary>
	/// 経路
	/// </summary>
	/// <returns>経路</returns>
	const vector<GSvector3>& path();

    /// <summary>
    /// 経路探索結果を破棄して終了する
    /// </summary>
    void end();

    /// <summary>
    /// 壁からどれだけ離すかを設定する
    /// </summary>
    /// <returns>参照 0.0~1.0で指定</returns>
    float& offset_ratio();

private:
	// ナビメッシュ
	NavMeshSurface* navmesh_{ nullptr };
    // 移動対象のアクター
    Actor* target_{ nullptr };
	// 経路
	vector<GSvector3> path_;
	// どこまで進んだか
	GSuint path_index_{ 0 };
    // ゴール地点
    GSvector3 goal_position_{ 0.0f, 0.0f, 0.0f };
    // ゴール地点とするアクター
    Actor* goal_actor_{ nullptr };
    // 再探索タイマー
    float re_find_timer_{ 0.0f };
    // 壁からどれだけ離すか 0.0~1.0で指定
    float offset_ratio_{ 0.0f };

};

#endif
