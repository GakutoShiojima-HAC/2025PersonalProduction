// -----------------------------------------------------------------------------------------
//  File        : CameraShake.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/10/19
//  Updated     : 2025/10/19
//  Description : カメラの揺れの値を管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_SHAKE_H_
#define CAMERA_SHAKE_H_

#include <gslib.h>
#include "Engine/Core/Camera/CameraShakeType.h"

class CameraShake {
public:
    CameraShake() = default;

    ~CameraShake() = default;

public:
    void update(float delta_time);

public:
    /// <summary>
    /// 揺らす
    /// </summary>
    /// <param name="type">= 揺れの種類</param>
    /// <param name="duration">= 揺らす時間(秒)</param>
    /// <param name="strength">= 揺れの強さ</param>
    /// <param name="loop">= ループするかどうか(時間を無視する)</param>
    void shake(CameraShakeType type, float duration, float strength = 1.0f, bool loop = false);

    /// <summary>
    /// 揺れを終了
    /// </summary>
    void end();

    /// <summary>
    /// 揺れの有効化
    /// </summary>
    /// <returns>参照</returns>
    bool& enable();

    /// <summary>
    /// 揺れによる加算すべきオフセットを取得
    /// </summary>
    /// <returns>オフセット</returns>
    GSvector3 get_offset() const;

private:
    /// <summary>
    /// ランダムな揺れ
    /// </summary>
    void update_random_shake(float delta_time);

    /// <summary>
    /// 縦揺れと横揺れ
    /// </summary>
    void update_shaking(float delta_time);

    /// <summary>
    /// 渦巻き揺れ
    /// </summary>
    void update_swirl(float delta_time);

    /// <summary>
    /// 手振れ
    /// </summary>
    void update_hand_shake(float delta_time);

private:
    bool enabled_{ true };

    // 揺れのタイプ
    CameraShakeType type_{ CameraShakeType::Shake };
    // 揺れの強さ
    float strength_{ 0.0f };
    // 揺れの時間
    float duration_{ 0.0f };
    // 経過時間
    float elapsed_{ 0.0f };
    // 揺れが終了したか
    bool is_finished_{ true };
    // ループするか
    bool is_loop_{ false };
    // 揺れ幅
    GSvector3 shake_offset_{ 0.0f, 0.0f, 0.0f };
    // 揺れ幅の保持
    GSvector3 swing_width_{ 0.0f, 0.0f, 0.0f };
    // ランダムな揺れ用のオフセット
    GSvector3 swing_random_offset_{ 0.0f, 0.0f, 0.0f };

public:
    // コピー禁止
    CameraShake(const CameraShake& other) = delete;
    CameraShake& operator = (const CameraShake& other) = delete;
};

#endif
