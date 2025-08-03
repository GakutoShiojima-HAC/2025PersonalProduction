// -----------------------------------------------------------------------------------------
//  File        : Camera.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/10
//  Updated     : 2025/07/10
//  Description : ゲーム内に登場するカメラの親クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Camera/CameraTag.h"
#include <GStransform.h>

class IWorld;

class Camera {
public:
	Camera() = default;

	virtual ~Camera() = default;

public:
	virtual void update(float delta_time);

	/// <summary>
	/// このカメラに切り替わった時の処理
	/// </summary>
	virtual void enter();

	/// <summary>
	/// 別のカメラに切り替わった時の処理
	/// </summary>
	virtual void exit();

	/// <summary>
	/// カメラの寿命を終わらせる
	/// </summary>
	virtual void die();

public:
	/// <summary>
	/// カメラタグを取得
	/// </summary>
	/// <returns>カメラタグ</returns>
	CameraTag& tag();

	/// <summary>
	/// トランスフォームを返却(const)
	/// </summary>
	const GStransform& transform() const;

	/// <summary>
	/// トランスフォームを返却
	/// </summary>
	GStransform& transform();

	/// <summary>
	/// カメラの寿命が尽きたかどうか
	/// </summary>
	bool is_dead() const;

	/// <summary>
	/// カメラが使われているかどうか
	/// </summary>
	bool& is_using();

protected:
	// ワールド
	IWorld* world_{ nullptr };
	// 寿命
	bool is_dead_{ false };
	// 使用中かどうか
	bool is_using_{ false };
	// カメラタグ
	CameraTag tag_{ CameraTag::None };
	// トランスフォーム
	GStransform transform_;

public:
	// コピー禁止
	Camera(const Camera& other) = delete;
	Camera& operator = (const Camera& other) = delete;

};

#endif
