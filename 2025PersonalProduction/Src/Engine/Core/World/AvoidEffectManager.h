#ifndef AVOID_EFFECT_MANAGER_H_
#define AVOID_EFFECT_MANAGER_H_

class World;

class AvoidEffectManager {
public:
    AvoidEffectManager(World* world = nullptr);

    ~AvoidEffectManager() = default;

public:
    void update(float delta_time);

    void clear();

public:
    /// <summary>
    /// 開始
    /// </summary>
    /// <param name="time">= 時間</param>
    /// <param name="time_scale">= タイムスケール (指定なしで変えない)</param>
    void start(float time, float time_scale = -1.0f);

    /// <summary>
    /// 一時停止
    /// </summary>
    /// <param name="time">= 再開までの時間 (指定なしで自動で再開しない)</param>
    void pause(float time = -1.0f);

    /// <summary>
    /// 再開
    /// </summary>
    void resume();

    /// <summary>
    /// 回避エフェクト中か？
    /// </summary>
    /// <returns></returns>
    bool is_effect() const;

private:
    enum class State {
        Idle,   // 待機中
        Pause,  // ポーズ中
        Effect, // エフェクト中
        Return  // エフェクト解除中
    };

private:
    World* world_{ nullptr };

    State state_{ State::Idle };

    float timer_{ 0.0f };

    float pause_timer_{ 0.0f };

    State prev_state_{ State::Idle };

};

#endif
