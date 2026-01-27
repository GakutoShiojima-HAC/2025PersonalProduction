#ifndef IMPACT_EFFECT_H_
#define IMPACT_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Impact {
        /// <summary>
        /// インパクトエフェクトを適用する
        /// </summary>
        /// <param name="source">= エフェクトを適用したいレンダーターゲット</param>
        /// <param name="threshold)">= 強さ</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint apply(GSuint source, float power);
    }
}

#endif
