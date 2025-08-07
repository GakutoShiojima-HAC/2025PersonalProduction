// -----------------------------------------------------------------------------------------
//  File          : NavMeshDebugger.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/06
//  Updated       : 2025/08/08
//  Description   : ナビメッシュデバッグクラス
//					ルートの表示、移動のデバッグが可能
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NAV_MESH_DEBUGGER_H_
#define NAV_MESH_DEBUGGER_H_

#include "Engine/Core/Actor/Actor.h"
#include "Engine/Core/NavMesh/NavMeshAgent.h"

class NavMeshDebugger : public Actor {
public:
	NavMeshDebugger(IWorld* world);

public:
	void update(float delta_time) override;

	void draw() const;

private:
	/// <summary>
	/// 経路に沿って移動を開始する
	/// </summary>
	void move_start();

	/// <summary>
	/// 経路に沿って移動する
	/// </summary>
	void update_move(float delta_time);

private:
	// ナビメッシュ
	NavMeshAgent navmesh_{};

	// 開始地点
	GSvector3 start_{ 1.0f, 0.5f, 0.0f };
	// 終了地点
	GSvector3 end_{ -1.0f, 0.5f, 0.0f };

	// 移動待機中かどうか
	bool is_move_waiting_{ true };

#ifdef _DEBUG
	// 直線経路だけを描画するかどうか
	bool draw_line_path_{ false };
#endif
};

#endif
