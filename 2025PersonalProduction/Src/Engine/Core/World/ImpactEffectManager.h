#ifndef IMPACT_EFFECT_MANAGER_H_
#define IMPACT_EFFECT_MANAGER_H_

class ImpactEffectManager {
public:
    ImpactEffectManager() = default;

    ~ImpactEffectManager() = default;

public:
    void update(float delta_time);

    void clear();

public:
    /// <summary>
    /// エフェクトの開始
    /// </summary>
    /// <param name="time">= エフェクトの時間</param>
    void start(float time);

    /// <summary>
    /// エフェクトが有効中かどうか
    /// </summary>
    /// <returns></returns>
    bool is_active() const;

private:
    float timer_{ 0.0f };

};

#endif
