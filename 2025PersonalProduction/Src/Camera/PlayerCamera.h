// -----------------------------------------------------------------------------------------
//  File          : PlayerCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/11
//  Updated       : 2025/08/11
//  Description   : プレイヤーカメラ
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PLAYER_CAMERA_H_
#define PLAYER_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Input/Input.h"

class Pawn;

class PlayerCamera : public Camera {
public:
	PlayerCamera(IWorld* world);

public:
	void update(float delta_time) override;

	void die() override;

public:
	/// <summary>
	/// 基点とするポーンを設定
	/// </summary>
	/// <param name="owner">= 基点</param>
	void set_owner(Pawn* owner);

	/// <summary>
	/// ロックオンターゲットとするポーンを設定
	/// </summary>
	/// <param name="target">= ターゲット</param>
	void set_lockon_target(Pawn* target);

	/// <summary>
	/// ロックオン中かどうか
	/// </summary>
	/// <returns>していたら真を返却</returns>
	bool is_lockon() const;

	/// <summary>
	/// ロックオン中のターゲットを返却
	/// </summary>
	/// <returns>していたらポーンを、していなければnullptr</returns>
	Pawn* get_lockon_target();

private:
	Input& input_ = Input::get_instance();

	// カメラの基点とするポーン
	Pawn* owner_{ nullptr };
	// ロックオンターゲットとするポーン
	Pawn* lockon_target_{ nullptr };

	// y軸周りの回転角度
	float yaw_{ 0.0f };
	// x軸周りの回転角度
	float pitch_{ 0.0f };

};
#endif
