#ifndef AVOID_BLUR_EFFECT_H_
#define AVOID_BLUR_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace AvoidBlur {
        /// <summary>
        /// 回避ブラーを適用する
        /// </summary>
        /// <param name="source">= エフェクトを適用したいレンダーターゲット</param>
        /// <param name="strength)">= ブラーの強さ</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint apply(GSuint source, float strength);
    }
}

#endif
