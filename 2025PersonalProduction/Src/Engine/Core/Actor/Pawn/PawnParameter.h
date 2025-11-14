#ifndef PAWN_PARAMETER_H_
#define PAWN_PARAMETER_H_

#include <string>

namespace PawnParameter {
    enum class Type {
        Normal,
        Big
    };

    /// <summary>
    /// 文字列からタイプを取得(パターンが少ないのでシンプルに実装)
    /// </summary>
    /// <param name="name">= 識別名</param>
    /// <returns>タイプ</returns>
    inline Type get_type(const std::string& name) {
        return name == "Big" ? Type::Big : Type::Normal;
    }

    /// <summary>
    /// 衝突判定の半径を取得
    /// </summary>
    /// <param name="type">= タイプ</param>
    /// <returns>半径</returns>
    inline float radius(Type type) {
        switch (type) {
        case PawnParameter::Type::Big: return 0.6f;
        default: return 0.4f;
        }
    }

    /// <summary>
    /// ポーンの身長を取得
    /// </summary>
    /// <param name="type">= タイプ</param>
    /// <returns>身長</returns>
    inline float height(Type type) {
        switch (type) {
        case PawnParameter::Type::Big:
            return 1.6f;
        default:
            return 1.6f;
        }
    }
}

#endif
