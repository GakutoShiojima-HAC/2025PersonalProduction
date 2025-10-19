// -----------------------------------------------------------------------------------------
//  File          : Vibration.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : コントローラーの振動を管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef VIBRATION_H_
#define VIBRATION_H_


class Vibration {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    Vibration() = default;

public:
    ~Vibration() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static Vibration& get_instance();

public:
    void update(float delta_time);

    /// <summary>
    /// 振動を開始
    /// </summary>
    /// <param name="time">= 時間(秒)</param>
    /// <param name="strength">= 強さ(0.0~1.0)</param>
    void start(float time, float strength);

    /// <summary>
    /// 振動を開始
    /// </summary>
    /// <param name="time">= 時間(秒)</param>
    /// <param name="strength_left">= 左モーターの強さ(0.0~1.0)</param>
    /// <param name="strength_right">= 右モーターの強さ(0.0~1.0)</param>
    void start(float time, float strength_left, float strength_right);

    /// <summary>
    /// 振動を終了
    /// </summary>
    void end();

private:
    float timer_{ 0.0f };

    float strength_{ 0.0f };

public:
    // コピー禁止
    Vibration(const Vibration& other) = delete;
    Vibration& operator = (const Vibration& other) = delete;

};

#endif
