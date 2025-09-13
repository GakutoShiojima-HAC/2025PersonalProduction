#ifndef BLUR_EFFECT_H_
#define BLUR_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Blur {
        /// <summary>
        /// ガウシアンブラーを適用する
        /// </summary>
        /// <param name="source">= ブラーをかけたいレンダーターゲット</param>
        /// <param name="size">= 縮小バッファのサイズ</param>
        /// <param name="blur_h">= 水平方向ブラー用のレンダーターゲット</param>
        /// <param name="blur_v">= 垂直方向ブラー用のレンダーターゲット</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint apply_blur(GSuint source, GSvector2 size, GSuint blur_h, GSuint blur_v);
    }
}

#endif
