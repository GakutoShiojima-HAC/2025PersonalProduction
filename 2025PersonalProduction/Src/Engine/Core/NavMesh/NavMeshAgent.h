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
#include "Engine/Core/Actor/Actor.h"

using namespace std;

class NavMeshSurface;

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
	/// 経路探索
	/// </summary>
	/// <param name="start">= 開始地点</param>
	/// <param name="end">= 終了地点</param>
    /// <param name="offset_ratio">= 壁からどれだけ離すか 0.0~1.0で指定</param>
	/// <returns>探索に成功したら真を返却</returns>
	bool find_path(const GSvector3& start, const GSvector3& end, float offset_ratio = 1.0f);

	/// <summary>
	/// 経路が見つかっているか
	/// </summary>
	/// <returns>見つかっていたら真を返却</returns>
	bool found_path() const;

	/// <summary>
	/// 移動が終了しているかどうか
	/// </summary>
	/// <returns>終了していたら真を返却</returns>
	bool end_move() const;

	/// <summary>
	/// 経路
	/// </summary>
	/// <returns>経路</returns>
	const vector<GSvector3>& path();

	/// <summary>
	/// 移動をリセットする
	/// </summary>
	void reset_move();

private:
	// ナビメッシュ
	NavMeshSurface* navmesh_{ nullptr };
	// 経路
	vector<GSvector3> path_;
	// どこまで進んだか
	GSuint path_index_{ 0 };
    // 再探索タイマー
    float re_find_timer_{ 0.0f };
    // ゴール地点
    GSvector3 goal_position_{ 0.0f, 0.0f, 0.0f };

	// 移動対象のアクター
	Actor* target_{ nullptr };

};

#endif
