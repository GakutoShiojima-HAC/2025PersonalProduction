#ifndef FOG_EFFECT_H_
#define FOG_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Fog {
        /// <summary>
        /// フォグを適用
        /// </summary>
        /// <param name="source">= 適用元のレンダーターゲット</param>
        /// <param name="color">= フォグの色</param>
        /// <param name="start">= フォグを適用する開始距離</param>
        /// <param name="end">= フォグを適用する終了距離</param>
        /// <returns></returns>
        GSuint fog(GSuint source, const GScolor& color, float start, float end);

        /// <summary>
        /// zバッファパラメータの作成
        /// </summary>
        /// <param name="near">= 近クリップ</param>
        /// <param name="far">= 遠クリップ</param>
        /// <returns>zバッファパラメータ</returns>
        GSvector4 zbuffer_params(float near, float far);
    }
}

#endif
