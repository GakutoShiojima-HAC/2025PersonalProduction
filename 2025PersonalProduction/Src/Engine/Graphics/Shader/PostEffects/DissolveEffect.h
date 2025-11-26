#ifndef DISSOLVE_EFFECT_H_
#define DISSOLVE_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Dissolve {
        /// <summary>
        /// ディゾルブエフェクトを適用する
        /// </summary>
        /// <param name="source">= エフェクトを適用したいレンダーターゲット</param>
        /// <param name="threshold)">= しきい値</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint apply(GSuint source, float threshold);
    }
}

#endif
