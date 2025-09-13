#ifndef SSAO_EFFECT_H_
#define SSAO_EFFECT_H_

#include <gslib.h>
#include <vector>

namespace PostEffect {
    namespace SSAO {
        /// <summary>
        /// アンビエントオクルージョン用マップを取得
        /// </summary>
        /// <param name="source">= シーンレンダーターゲット</param>
        /// <param name="projection">= 射影行列</param>
        /// <param name="noise_texture">= ノイズテクスチャ</param>
        /// <param name="sample_kernel">= サンプルカーネル</param>
        /// <param name="screen_size">= スクリーンサイズ</param>
        /// <returns>アンビエントオクルージョン用マップのレンダーターゲット</returns>
        GSuint get_aomap(
            GSuint source,
            const GSmatrix4& projection,
            GSint noise_texture,
            const std::vector<GSvector3>& sample_kernel,
            const GSvector2& screen_size
        );

        /// <summary>
        /// アンビエントオクルージョンを合成
        /// </summary>
        /// <param name="source">= 合成元のレンダーターゲット</param>
        /// <param name="aomap">= アンビエントオクルージョン用マップ</param>
        /// <returns>加工したレンダーターゲット</returns>
        GSuint combine(GSuint source, GSuint aomap);

        /// <summary>
        /// サンプルカーネルを作成
        /// </summary>
        /// <param name="sample_kernel">= 作成先</param>
        /// <param name="size">= サイズ</param>
        void create_sample_kernel(std::vector<GSvector3>& sample_kernel, int size);

        /// <summary>
        /// ノイズテクスチャを作成
        /// </summary>
        /// <param name="texture">= 作成先</param>
        void create_noise_texture(int& texture);
    }
}

#endif
