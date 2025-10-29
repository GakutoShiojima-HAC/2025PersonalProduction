// -----------------------------------------------------------------------------------------
//  File          : EditorCamera.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/04
//  Updated       : 2025/08/04
//  Description   : 編集用カメラ
//					Activeにした状態で右クリックを押している間だけ動かすことができる
//					このカメラを使ってデバッグやタイムラインの編集を行う
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef EDITOR_CAMERA_H_
#define EDITOR_CAMERA_H_

#include "Engine/Core/Camera/Camera.h"
#include "Engine/Core/Input/Input.h"

class EditorCamera : public Camera {
public:
	EditorCamera(IWorld* world);

public:
	void update(float delta_time) override;

	void exit() override;

	void die() override;

private:
	/// <summary>
	/// カメラタイムラインが再生中かどうか
	/// </summary>
	/// <returns>再生中なら真を返却</returns>
	bool is_play_timeline() const;

	/// <summary>
	/// カメラの切り替え
	/// </summary>
	void toggle_camera();

private:
	Input& input_ = Input::get_instance();

	// 元のカメラ
	Camera* prev_{ nullptr };

	// エディタカメラを使えるかどうか
	bool is_active_{ false };

	// y軸周りの回転角度
	float yaw_{ 0.0f };
	// x軸周りの回転角度
	float pitch_{ 0.0f };
	// 傾き
	float angle_{ 0.0f };

};

#endif
