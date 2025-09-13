#ifndef MASK_EFFECT_H_
#define MASK_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Mask {
        /// <summary>
        /// マスクをかける
        /// </summary>
        /// <param name="source">= マスク元のレンダーターゲット</param>
        /// <param name="mask">= マスク用レンダーターゲット</param>
        /// <param name="color">= マスク色</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint mask(GSuint source, GSuint mask, const GScolor& color);
    }
}

#endif
