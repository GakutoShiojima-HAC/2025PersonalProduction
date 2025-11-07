// -----------------------------------------------------------------------------------------
//  File        : GameTimer.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/10/20
//  Updated     : 2025/10/20
//  Description : ゲームの経過時間を管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef GAME_TIMER_H_
#define GAME_TIMER_H_

class GameTimer {
public:
    GameTimer() = default;

    ~GameTimer() = default;

public:
    /// <param name="active">= タイマーを使うかどうか</param>
    void init(bool enable);

    void update(float delta_time);

    void draw() const;

    /// <summary>
    /// 経過時間(秒)を取得
    /// </summary>
    /// <returns>時間</returns>
    float get_elapsed_time() const;

private:
    bool enabled_{ true };

    float elapsed_time_{ 0.0f };

public:
    // コピー禁止
    GameTimer(const GameTimer& other) = delete;
    GameTimer& operator = (const GameTimer& other) = delete;
};

#endif
