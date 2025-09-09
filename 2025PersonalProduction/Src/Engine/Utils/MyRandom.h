#ifndef MY_RANDOM_H_
#define MY_RANDOM_H_

#include <random>
#include <gslib.h>

namespace MyRandom {
    /// <summary>
    /// ランダムエンジンを取得
    /// </summary>
    /// <returns>参照</returns>
    inline std::mt19937& get_engine() {
        static thread_local std::mt19937 engine(std::random_device{}());
        return engine;
    }

    /// <summary>
    /// シード値を設定する
    /// </summary>
    /// <param name="seed">= 設定するシード値</param>
    /// <returns>設定したシード値</returns>
    inline unsigned int set_seed(unsigned int seed) {
        get_engine().seed(seed);
        return seed;
    }

    inline int random_int(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(get_engine());
    }

    inline float random_float(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(get_engine());
    }

    inline GSvector2 random_vec2(GSvector2 min, GSvector2 max) {
        return GSvector2{ random_float(min.x, max.x), random_float(min.y, max.y) };
    }

    inline GSvector3 random_vec3(GSvector3 min, GSvector3 max) {
        return GSvector3{ random_float(min.x, max.x), random_float(min.y, max.y), random_float(min.z, max.z) };
    }
}

#endif
