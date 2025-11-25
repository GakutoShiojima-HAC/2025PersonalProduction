// -----------------------------------------------------------------------------------------
//  File        : ActionScore.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/10/18
//  Updated     : 2025/10/19
//  Description : アクションによる評価を管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ACTION_SCORE_H_
#define ACTION_SCORE_H_

#include <string>

class ActionScore {
public:
    ActionScore() = default;

    ~ActionScore() = default;

public:
    void init();

    void update(float delta_time);

    void draw() const;

public:
    /// <summary>
    /// スコアを追加
    /// </summary>
    /// <param name="basic_score">= 基礎スコア(等倍)</param>
    /// <param name="combo_time">= コンボが途切れるまでの時間(delta_time)</param>
    /// <param name="bonus_progress">= 倍率上昇までのカウンター(1.0 == 100%で上昇)</param>
    void add_score(int basic_score, float combo_time, float bonus_progress);

    /// <summary>
    /// アクションテキストを設定
    /// </summary>
    /// <param name="text">= テキスト</param>
    void set_action_text(const std::string& text);

public:
    /// <summary>
    /// 総スコアを取得
    /// </summary>
    /// <returns>総スコア</returns>
    int get_total_score() const;

    /// <summary>
    /// 最大コンボスコアを取得
    /// </summary>
    /// <returns>最大コンボスコア</returns>
    int get_max_combo_score() const;

    /// <summary>
    /// 最大コンボ数を取得
    /// </summary>
    /// <returns></returns>
    int get_max_combo_count() const;

private:
    /// <summary>
    /// コンボボーナスをリセット
    /// </summary>
    void reset_combo_bonus();

private:
    // 総スコア
    int total_score_{ 0 };
    // コンボスコア
    int combo_score_{ 0 };
    // コンボボーナスの残り時間タイマー
    float combo_timer_{ 0.0f };
    // コンボボーナスの時間
    float combo_time_{ 0.0 };
    // コンボボーナス倍率
    float combo_bonus_{ 1.0f };
    // コンボボーナス倍率の上昇カウンター
    float combo_bonus_progress_{ 0.0f };
    // アクションテキスト
    std::string action_text_{ "" };
    // アクションテキスト表示タイマー
    float action_text_timer_{ 0.0f };
    // 最高コンボスコア
    int max_combo_score_{ 0 };
    // 連続コンボ数
    int combo_count_{ 0 };
    // 最高連続コンボ数
    int max_combo_count_{ 0 };

    // コンボスコアの強調表示タイマー
    float combo_emphasis_timer_{ 0.0f };
    // コンボボーナス倍率の強調表示タイマー
    float combo_bonus_emphasis_timer_{ 0.0f };

    // 音を鳴らすかどうか
    bool play_add_score_se_{ false };

public:
    // コピー禁止
    ActionScore(const ActionScore& other) = delete;
    ActionScore& operator = (const ActionScore& other) = delete;
};

#endif
