// -----------------------------------------------------------------------------------------
//  File        : CameraManager.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/10
//  Updated     : 2025/09/13
//  Description : ゲーム内に登場するカメラを管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_

#include <unordered_map>
#include "Camera/CameraTag.h"

using namespace std;

class Camera;
struct ScreenData;
struct GSmatrix4;

class CameraManager {
public:
	CameraManager();

	virtual ~CameraManager();

public:
	void update(float delta_time);

	virtual void draw() const;

	/// <summary>
	/// カメラを管理下に追加
	/// </summary>
	/// <param name="camera">カメラ</param>
	void add(Camera* camera);
	
	/// <summary>
	/// 管理下の全てのカメラを管理対象から外す
	/// </summary>
	void clear();

	/// <summary>
	/// 現在使用しているカメラを返却
	/// </summary>
	/// <returns>カメラ</returns>
	Camera* current() const;

	/// <summary>
	/// 遷移元に使用しているカメラを返却
	/// </summary>
	/// <returns>使用していればカメラを、していなければnullptr</returns>
	Camera* prev() const;

	/// <summary>
	/// カメラを検索
	/// </summary>
	/// <param name="tag">カメラタグ</param>
	/// <returns>見つかればカメラを、見つからなければnullptr</returns>
	Camera* find(const CameraTag tag) const;

	/// <summary>
	/// 現在のカメラから指定したカメラに遷移
	/// </summary>
	/// <param name="to">= 遷移先</param>
	/// <param name="time">= 遷移にかかる時間</param>
	void transition(Camera* to, float time = 0);

	/// <summary>
	/// 指定したカメラから指定したカメラに遷移
	/// </summary>
	/// <param name="from">= 遷移元</param>
	/// <param name="to">= 遷移先</param>
	/// <param name="time">= 遷移にかかる時間</param>
	void transition(Camera* from, Camera* to, float time = 0);

    /// <summary>
    /// 視野角を取得
    /// </summary>
    /// <returns>参照</returns>
    float& fov();

    /// <summary>
    /// 射影行列の取得
    /// </summary>
    /// <returns>現在の射影行列</returns>
    GSmatrix4 get_projection_matrix() const;

protected:
	/// <summary>
	/// 管理下の寿命が尽きたカメラを管理対象から外す
	/// </summary>
	void remove();

	/// <summary>
	/// カメラがないときの描画処理
	/// </summary>
	virtual void draw_empty() const;

	/// <summary>
	/// カメラの座標、方向、角度を取得
	/// </summary>
	/// <param name="pos">= カメラ座標</param>
	/// <param name="at">= カメラ方向</param>
	/// <param name="up">= カメラ角度</param>
	void camera_lookat(GSvector3& pos, GSvector3& at, GSvector3& up) const;

protected:
	// カメラマップ
	unordered_map<GSuint, Camera*> cameras_;
	// 現在使用しているカメラ
	Camera* current_{ nullptr };
	// トランジション用の以前に使っていたカメラ
	Camera* prev_{ nullptr };

    // スクリーンデータ
    ScreenData* screen_data_{ nullptr };

    // 視野角
    float fov_{ 45.0f };

	// トランジションにかかる時間
	float transition_time_{ 0.0f };
	// トランジションタイマー
	float transition_timer_{ 0.0f };

public:
	// コピー禁止
	CameraManager(const CameraManager& other) = delete;
	CameraManager& operator = (const CameraManager& other) = delete;

};

#endif
