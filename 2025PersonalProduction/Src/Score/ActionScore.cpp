#include "Score/ActionScore.h"
#include <sstream>
#include <locale>
#include <gslib.h>
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Utils/MyString.h"
#include "Assets.h"
#include "GameConfig.h"

// コンボボーナス倍率の上昇幅
const float BONUS_UP{ 0.5f };
// コンボボーナス倍率の最大値
const float BONUS_MAX{ 5.0f };

// コンボスコア強調表示の時間(秒)
const float COMBO_EMPHASIS_TIME{ 0.125f };
// コンボボーナス倍率強調表示の時間(秒)
const float COMBO_BONUS_EMPHASIS_TIME{ 0.12f };

void ActionScore::init() {
    total_score_ = 0;
    reset_combo_bonus();
    action_text_ = "";
    max_combo_score_ = 0;
}

void ActionScore::update(float delta_time) {
    // アクションテキストの表示タイマー
    if (action_text_timer_ > 0.0f) action_text_timer_ -= delta_time / cFPS;

    // コンボボーナス倍率タイマー
    if (combo_timer_ > 0.0f) {
        combo_timer_ -= delta_time;
        // コンボボーナス時間が終わってしまったらリセット
        if (combo_timer_ <= 0.0f) reset_combo_bonus();
    }
    // コンボスコア強調表示タイマー
    if (combo_emphasis_timer_ > 0.0f) combo_emphasis_timer_ -= delta_time / cFPS;
    // コンボボーナス倍率の強調表示タイマー
    if (combo_bonus_emphasis_timer_ > 0.0f) combo_bonus_emphasis_timer_ -= delta_time / cFPS;

    // SE
    if (play_add_score_se_) {
        gsPlaySE((GSuint)SEID::AddScore);
        play_add_score_se_ = false;
    }
}

void ActionScore::draw() const {
    // 総スコア
    if (total_score_ > 0) {
        // 桁区切りの文字列を生成
        std::stringstream ss;
        ss.imbue(std::locale(""));
        ss << total_score_;

        Canvas::draw_sprite_text(
            ss.str(),
            GSvector2{ 60.0f, 400.0f },
            40,
            cFONT,
            GS_FONT_BOLD
        );
    }

    // コンボ
    if (combo_timer_ > 0.0f) {
        // コンボスコア
        {
            // 桁区切りの文字列を生成
            std::stringstream ss;
            ss.imbue(std::locale(""));
            ss << combo_score_;
            // 強調表示の進捗率
            const float progress = CLAMP(combo_emphasis_timer_ / COMBO_EMPHASIS_TIME, 0.0f, 1.0f);

            Canvas::draw_sprite_text(
                "+" + ss.str(),
                GSvector2{ 710.0f, 180.0f },
                80 + 10 * progress,
                cFONT,
                GS_FONT_BOLD,
                GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
                Anchor::TopLeft,
                Anchor::TopRight
            );
        }

        // コンボ倍率
        {
            // 小数点の位置を取得
            std::string s = "x" + std::to_string(combo_bonus_);
            size_t dot_pos = s.find('.');
            // 小数点があれば指定位置より先は切り捨て
            if (dot_pos != std::string::npos) s = s.substr(0, dot_pos + 2);
            // 強調表示の進捗率
            const float progress = CLAMP(combo_bonus_emphasis_timer_ / COMBO_BONUS_EMPHASIS_TIME, 0.0f, 1.0f);

            Canvas::draw_sprite_text(
                s,
                GSvector2{ 700.0f, 255.0f },
                30 + 10 * progress,
                cFONT,
                GS_FONT_BOLD,
                GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
                Anchor::TopLeft,
                Anchor::TopRight
            );
        }

        // 残り時間ゲージ
        {
            // 強調表示の進捗率
            const float progress = CLAMP(combo_timer_ / combo_time_, 0.0f, 1.0f);

            Canvas::draw_texture(
                (GSuint)TextureID::BonusGauge,
                GSvector2{ 435.0f, 264.0f },
                GSrect{ 0, 0, 200 * progress, 15 }
            );
        }
    }

    // アクションテキスト
    if (action_text_timer_ > 0.0f) {
        Canvas::draw_sprite_text(
            MyString::utf8_to_shiftjis(action_text_),
            GSvector2{ 550.0f, 284.0f },
            60,
            cFONT,
            GS_FONT_BOLD,
            GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
            Anchor::TopLeft,
            Anchor::TopCenter
        );
    }
}

void ActionScore::add_score(int basic_score, float combo_time, float bonus_progress) {
    float score = (float)basic_score * combo_bonus_;
    // 総スコアを加算
    total_score_ += (int)score;
    // コンボスコアを加算
    combo_score_ += (int)score;
    combo_emphasis_timer_ = COMBO_EMPHASIS_TIME;
    combo_count_ += 1;
    // コンボボーナス時間を設定
    combo_timer_ = combo_time_ = combo_time;
    // コンボボーナス倍率上昇カウンターを加算
    combo_bonus_progress_ += bonus_progress;
    // SE
    play_add_score_se_ = true;
    // 上昇分を満たせていたら倍率を上昇
    if (combo_bonus_progress_ >= 1.0f) {
        combo_bonus_progress_ -= 1.0f;
        combo_bonus_ += BONUS_UP;
        if (combo_bonus_ >= BONUS_MAX) combo_bonus_ = BONUS_MAX;
        else combo_bonus_emphasis_timer_ = COMBO_BONUS_EMPHASIS_TIME;
    }
    // もしコンボ数が5の倍率ならテキスト描画
    //if (combo_count_ % 5 == 0) set_action_text(std::to_string(combo_count_) + "Combo!");
}

void ActionScore::set_action_text(const std::string& text) {
    action_text_ = text;
    // 空白文字でなければ表示する
    if (action_text_ != "") action_text_timer_ = 2.0f;
}

int ActionScore::get_total_score() const {
    return total_score_;
}

int ActionScore::get_max_combo_score() const {
    return max_combo_score_;
}

int ActionScore::get_max_combo_count() const {
    return combo_count_;
}

void ActionScore::reset_combo_bonus() {
    // 最大コンボスコアを上書き
    if (combo_score_ > max_combo_score_) max_combo_score_ = combo_score_;

    combo_score_ = 0;
    combo_timer_ = 0.0f;
    combo_time_ = 0.0f;
    combo_bonus_ = 1.0f;
    combo_bonus_progress_ = 0.0f;
    combo_count_ = 0;

    combo_emphasis_timer_ = 0.0f;
    combo_bonus_emphasis_timer_ = 0.0f;
}
