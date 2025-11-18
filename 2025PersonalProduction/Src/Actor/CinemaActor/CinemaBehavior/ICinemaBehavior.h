// -----------------------------------------------------------------------------------------
//  File        : ICinemaBehavior.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/11/18
//  Updated     : 2025/11/18
//  Description : シネマアクターの行動定義 
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef I_CINEMA_BEHAVIOR_H_
#define I_CINEMA_BEHAVIOR_H_

class IWorld;
class Actor;

class ICinemaBehavior {
public:
    ICinemaBehavior(IWorld* world) : world_{ world } {}

    virtual ~ICinemaBehavior() = default;

public:
    virtual void update(float delta_time) {};

    virtual void draw() const {};

    virtual void draw_gui() const {};

    virtual void react(Actor& other) {};

public:
    /// <summary>
    /// 再生を開始できるかどうか
    /// </summary>
    /// <returns>開始できるなら真を返却</returns>
    bool is_start() const { return is_start_; }

    /// <summary>
    /// 再生を開始
    /// </summary>
    void enter() { is_play_ = true; }

protected:
    // 再生を開始できるかどうか
    bool is_start_{ false };
    // 再生中かどうか
    bool is_play_{ false };

protected:
    IWorld* world_{ nullptr };

public:
    // コピー禁止
    ICinemaBehavior(const ICinemaBehavior& other) = delete;
    ICinemaBehavior& operator = (const ICinemaBehavior& other) = delete;

};

#endif
