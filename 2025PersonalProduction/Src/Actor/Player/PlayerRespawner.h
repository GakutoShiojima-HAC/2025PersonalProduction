#ifndef PLAYER_RESPAWNER_H_
#define PLAYER_RESPAWNER_H_

#include <gslib.h>

class PlayerRespawner {
public:
    PlayerRespawner() = default;

    ~PlayerRespawner();

public:
    void clear();

    /// <summary>
    /// チェックポイントを追加
    /// </summary>
    /// <param name="position">= 復活したときの座標</param>
    /// <param name="rotate">= 復活したときの回転</param>
    void add_point(const GSvector3& position, const GSvector3& rotate);

public:
    /// <summary>
    /// リスポーンできるかどうか
    /// </summary>
    /// <returns>リスポーンできるなら真を返却</returns>
    bool can_respawn() const;

    /// <summary>
    /// 復活時の座標を取得
    /// </summary>
    /// <returns>座標</returns>
    GSvector3 respawn_position() const;

    /// <summary>
    /// 復活時の回転を取得
    /// </summary>
    /// <returns>回転</returns>
    GSvector3 respawn_rotate() const;

    /// <summary>
    /// リスポーン回数をカウントアップ
    /// </summary>
    void respawn_countup();

    /// <summary>
    /// リスポーン回数を取得
    /// </summary>
    /// <returns></returns>
    int respawn_count() const;

private:
    bool can_respawn_{ false };

    GSvector3 respawn_position_{ 0.0f, 0.0f, 0.0f };

    GSvector3 respawn_rotate_{ 0.0f, 0.0f, 0.0f };

    int counter_{ 0 };

public:
    // コピー禁止
    PlayerRespawner(const PlayerRespawner& other) = delete;
    PlayerRespawner& operator = (const PlayerRespawner& other) = delete;

};

#endif
