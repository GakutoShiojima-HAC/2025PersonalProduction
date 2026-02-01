#ifndef IMPACT_EFFECT_H_
#define IMPACT_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Impact {
        /// <summary>
        /// インパクトエフェクトを適用する
        /// </summary>
        /// <param name="source">= エフェクトを適用したいレンダーターゲット</param>
        /// /// <param name="depth">= 深度レンダーターゲット</param>
        /// <param name="threshold">= 強さ</param>
        /// <param name="aspect_ratio">= アスペクト比</param>
        /// <param name="hit">= インパクト位置(スクリーン座標)</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint apply(GSuint source, GSuint depth, float power, float aspect_ratio, const GSvector2& hit);
    }
}

#endif
