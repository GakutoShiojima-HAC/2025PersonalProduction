#ifndef FXAA_EFFECT_H_
#define FXAA_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace FXAA {
        /// <summary>
        /// アンチエイリアシングを適用
        /// </summary>
        /// <param name="source">= 適用元のレンダーターゲット</param>
        /// <param name="screen_size">= スクリーンサイズ</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint fxaa(GSuint source, const GSvector2& screen_size);
    }
}

#endif
