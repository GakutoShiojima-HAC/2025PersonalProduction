#ifndef BLOOM_EFFECT_H_
#define BLOOM_EFFECT_H_

#include <gslib.h>

namespace PostEffect {
    namespace Bloom {
        /// <summary>
        /// ブルームに使う高輝度テクセルの抽出
        /// </summary>
        /// <param name="source">= 抽出元のレンダーターゲット</param>
        /// <param name="threshold">= 対象とする高輝度テクセルのしきい値</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint extract(GSuint source, float threshold);

        /// <summary>
        /// ブルームエフェクトを合成
        /// </summary>
        /// <param name="source">= 合成元のレンダーターゲット</param>
        /// <param name="Intensity">= 強さ</param>
        /// <param name="blur1">= ブラーテクスチャ1</param>
        /// <param name="blur2">= ブラーテクスチャ2</param>
        /// <param name="blur3">= ブラーテクスチャ3</param>
        /// <param name="blur4">= ブラーテクスチャ4</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint combine(GSuint source, float intensity, GSuint blur1, GSuint blur2, GSuint blur3, GSuint blur4);
    }
}

#endif
