// -----------------------------------------------------------------------------------------
//  File          : Character.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/10
//  Updated       : 2025/07/10
//  Description   : ステートマシーンを持つポーンの親
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_H_
#define CHARACTER_H_

#include "Engine/Core/Actor/Pawn/Pawn.h"
#include "Engine/Core/StateMachine/StateMachine.h"

class Character : public Pawn {
public:
	Character() = default;

	virtual ~Character() = default;

public:
	/// <summary>
	/// ステートの変更
	/// </summary>
	/// <param name="state_num">= ステート番号</param>
	void change_state(const GSuint state_num);

	/// <summary>
	/// ステートの変更(モーション指定あり)
	/// </summary>
	/// <param name="state_num">= ステート番号</param>
	/// <param name="motion_num">= モーション番号</param>
	/// <param name="loop">= ループするかどうか</param>
	void change_state(const GSuint state_num, const GSuint motion_num, const bool loop);

    /// <summary>
    /// 現在のステートに入ってからの経過時間
    /// </summary>
    /// <returns>delta_time</returns>
    float state_timer() const;

    /// <summary>
    /// 現在のステート番号
    /// </summary>
    /// <returns>ステート番号</returns>
    GSuint current_state_num() const;

protected:
	/// <summary>
	/// ステートの追加
	/// </summary>
	virtual void add_state() = 0;

protected:
	/// <summary>
	/// ステートマシーンの更新
	/// </summary>
	void update_state(float delta_time);

protected:
	// ステートマシン
	StateMachine state_;
	// 状態タイマー
	float state_timer_{ 0.0f };

};

#endif
