// ---------------------------------------------------------------------------------------- -
//  File        : CameraShakeType.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/10/19
//  Updated     : 2025/10/19
//  Description : カメラの揺れの種類
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CAMERA_SHAKE_TYPE_H_
#define CAMERA_SHAKE_TYPE_H_

enum class CameraShakeType {
    Shake,              // ランダムな揺れ
    ShakeV,             // 横揺れ
    ShakeH,             // 縦揺れ
    SwirlClock,         // 渦巻き(時計回り)
    SwirlCounterClock,  // 渦巻き(反時計回り)
    HandShake,          // 手振れのような揺れ
};

#endif
