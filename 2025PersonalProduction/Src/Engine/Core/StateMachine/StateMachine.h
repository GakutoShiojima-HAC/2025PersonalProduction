// -----------------------------------------------------------------------------------------
//  File        : StateMachine.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/25
//  Description : ステートマシーンによる状態管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <memory>
#include <unordered_map>
#include "Engine/Core/StateMachine/IState.h"

typedef	unsigned int GSuint;

using namespace std;

class StateMachine {
public:
	StateMachine();

	~StateMachine();

public:
	void update(float delta_time);

	void draw() const;

	void draw_gui() const;

	void clear();

public:
	/// <summary>
	/// 状態の追加
	/// </summary>
	/// <param name="state_num">= 状態番号</param>
	/// <param name="state">= 状態</param>
	void add_state(const GSuint state_num, shared_ptr<IState> state);
	
	/// <summary>
	/// 状態を変更
	/// </summary>
	/// <param name="state_num">= 変更先の状態番号</param>
	void change_state(const GSuint state_num);

	/// <summary>
	/// 指定した状態が現在の状態と一致するかどうか
	/// </summary>
	/// <param name="state_num">= 状態番号</param>
	/// <returns>一致するなら真</returns>
	bool is_current_state(const GSuint state_num) const;

	/// <summary>
	/// 現在の状態番号を返却
	/// </summary>
	/// <returns>状態番号</returns>
	GSuint get_current_state() const;

	/// <summary>
	/// 指定した状態を返却
	/// </summary>
	/// <param name="state_num">= 状態番号</param>
	/// <returns>見つかれば状態を、見つからなければnullptr</returns>
	IState* get_state(const GSuint state_num) const;

private:
	/// <summary>
	/// 指定した状態を検索
	/// </summary>
	/// <returns>見つかれば状態を、見つからなければnullptr</returns>
	shared_ptr<IState> find(const GSuint state_num) const;

private:
	// 状態
	unordered_map<GSuint, shared_ptr<IState>> state_list_;

	// 現在の状態
	shared_ptr<IState> current_state_;

	// 現在の状態番号
	GSuint current_state_num_{ 0 };

public:
	// コピー禁止
	StateMachine(const StateMachine& other) = delete;
	StateMachine& operator = (const StateMachine& other) = delete;

};

#endif
