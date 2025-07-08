// -----------------------------------------------------------------------------------------
//  File        : IState.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/06/25
//  Description : ステートのインターフェース
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef I_STATE_H_
#define I_STATE_H_

class IState {
public:
	IState() = default;

	virtual ~IState() = default;

public:
	/// <summary>
	/// 状態開始
	/// </summary>
	virtual void enter() = 0;
	
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="delta_time">= 前フレームからの経過フレーム数</param>
	virtual void update(float delta_time) = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void draw() const = 0;

	/// <summary>
	/// GUIの描画
	/// </summary>
	virtual void draw_gui() const = 0;

	/// <summary>
	/// 状態終了
	/// </summary>
	virtual void exit() = 0;

};

#endif