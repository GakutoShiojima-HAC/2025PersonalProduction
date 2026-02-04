#include "SceneSelectSaveDataState.h"
#include "Scene/MenuScene.h"
#include "Engine/Utils/MyString.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core//Input/Input.h"
#include "GUI/Button/TextFunctionButton.h"
#include "Assets.h"
#include "Engine/Sound/SE.h"

// 何個UIを描画するか
const int DRAW_DATA_LIST{ 4 };

const GScolor DEFAULT_COLOR{ 1.0f, 1.0f, 1.0f, 1.0f };
const GScolor HIGHLIGHT_COLOR{ 1.0f, 0.85f, 0.0f, 1.0f };

const GSvector2 ARROW_POSITION{ 320.0f, 280.0f };
const GSvector2 FILENAME_POSITION{ 407.0f, 270.0f };
const GSvector2 LASTPLAY_POSITION{ 960.0f, 277.0f };
const GSvector2 STAGE_POSITION{ 407.0f, 327.0f };
const GSvector2 LEVEL_POSITION{ 960.0f, 327.0f };
const GSvector2 BAR_POSITION{ 287.0f, 380.0f };

static const float ICON_ROTATE_SPEED{ 1.0f };

SceneSelectSaveDataState::SceneSelectSaveDataState(MenuScene& owner) :
    owner_{ owner } {
    {
        TextFunctionButton* button = new TextFunctionButton{ "戻る", GSvector2{ 0.0f, 908.0f }, 60, Anchor::TopCenter, Anchor::Center };
        button->on_input([=] {
            return_state();
            });
        button_empty_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "戻る", GSvector2{ 100.0f, 908.0f }, 60, Anchor::TopCenter, Anchor::Center };
        button->on_input([=] {
            return_state();
            });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "開始", GSvector2{ -100.0f, 908.0f }, 60, Anchor::TopCenter, Anchor::Center };
        button->on_input([=] {
            start_game();
            });
        button_.add(button);
    }
}

void SceneSelectSaveDataState::enter() {
    button_.start();
    button_empty_.start();

    select_index_ = owner_.get_save_data().empty() ? -1 : 0;
}


void SceneSelectSaveDataState::update(float delta_time) {
    Input& input = Input::get_instance();
    const bool is_pad = input.is_pad();

    const std::vector<MenuSceneSaveDataInfo>& infos = owner_.get_save_data();

    if (is_pad) {
        if (input.action(InputAction::MENU_Decision)) {
            start_game();
            SE::play((GSuint)SEID::ButtonInput);
        }
        if (input.action(InputAction::MENU_Cancel)) {
            return_state();
            SE::play((GSuint)SEID::ButtonInput);
        }
    }
    else {
        button_.update(delta_time);
        button_empty_.update(delta_time);
    }

    if (!infos.empty()) {    
        int index = (int)select_index_;
        if (input.action(InputAction::GAME_Interact_Up)) {
            --index;
            SE::play((GSuint)SEID::ButtonSelect);
            if (index < 0) index = 0;
        }
        else if (input.action(InputAction::GAME_Interact_Down)) {
            ++index;
            SE::play((GSuint)SEID::ButtonSelect);
            if (index > infos.size() - 1) index = infos.size() - 1;
        }
        select_index_ = index;
    }

    // データアイコンの回転
    rotate_icon_ += delta_time * ICON_ROTATE_SPEED;
}

void SceneSelectSaveDataState::draw() const {
    owner_.original_draw();

    // 背景の描画
    Canvas::draw_texture(
        (GSuint)TextureID::SelectSaveDataWindowTexture,
        GSvector2{ 0.0f, 0.0f },
        GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f }
    );

    if (owner_.get_save_data().empty()) {
        draw_empty_save_data();
    }
    else {
        draw_select_save_data();
    }

    // データアイコンの描画
    Canvas::draw_texture(
        (GSuint)TextureID::SaveDataIconTexture,
        GSvector2{ 272.0f, 188.0f },
        GSrect{ 0.0f, 0.0f, 96.0f, 96.0f },
        GSvector2{ 96.0f / 2.0f, 96.0f / 2.0f },
        GSvector2::one(),
        GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
        rotate_icon_
    );

    Input::get_instance().draw_cursor();
}

void SceneSelectSaveDataState::draw_empty_save_data() const {
    button_empty_.draw();
}

void SceneSelectSaveDataState::draw_select_save_data() const {
    if (!Input::get_instance().is_pad()) {
        button_.draw();
    }

    const bool is_pad = Input::get_instance().is_pad();
    Canvas::draw_texture(is_pad ? (GSuint)TextureID::ButtonSelect2GamePadTexture : (GSuint)TextureID::ButtonSelect2MouseTexture,
        GSvector2{ 284.0f, 877.0f }, is_pad ? GSrect{ 0.0f, 0.0f, 600.0f, 64.0f } : GSrect{ 0.0f, 0.0f, 335.0f, 64.0f });

    const std::vector<MenuSceneSaveDataInfo>& infos = owner_.get_save_data();
    // 先頭
    const int first_index = select_index_ > DRAW_DATA_LIST - 1 ? select_index_ - (DRAW_DATA_LIST - 1) : 0;
    // 終端
    const int last_index = owner_.get_save_data().size() - 1;

    for (size_t i = 0; i < DRAW_DATA_LIST; ++i) {
        const int index = (int)i + first_index;
        const MenuSceneSaveDataInfo& data = infos[index];
        // テキストを取得
        const bool is_select = index == select_index_;
        std::string file_name = data.name;
        std::string time = "最終プレイ: " + data.time;
        std::string stage = data.stage < 0 ? "チュートリアル中" : "ステージ到達: " + std::to_string(data.stage);
        std::string level = "プレイヤーレベル: " + std::to_string(data.level);
        const GScolor color = is_select ? HIGHLIGHT_COLOR : DEFAULT_COLOR;
        const GSvector2 ADD_POSITION = GSvector2{ 0.0f, 130.0f * i };
        if (is_select) {
            Canvas::draw_sprite_text(
                ">>",
                ARROW_POSITION + ADD_POSITION,
                80.0f,
                cFONT,
                GS_FONT_NORMAL,
                color
            );
        }
        // テキストを描画
        Canvas::draw_sprite_text(
            file_name,
            FILENAME_POSITION + ADD_POSITION,
            54.0f,
            cFONT,
            GS_FONT_NORMAL,
            color
        );
        Canvas::draw_sprite_text(
            time,
            LASTPLAY_POSITION  + ADD_POSITION,
            44.0f,
            cFONT,
            GS_FONT_NORMAL,
            color
        );
        Canvas::draw_sprite_text(
            stage,
            STAGE_POSITION + ADD_POSITION,
            44.0f,
            cFONT,
            GS_FONT_NORMAL,
            color
        );
        Canvas::draw_sprite_text(
            level,
            LEVEL_POSITION + ADD_POSITION,
            44.0f,
            cFONT,
            GS_FONT_NORMAL,
            color
        );
      
        if (i < DRAW_DATA_LIST - 1) {
            Canvas::draw_texture(
                (GSuint)TextureID::SaveDataBarTexture,
                BAR_POSITION + ADD_POSITION,
                GSrect{ 0.0f, 0.0f, 1346.0f, 5.0f }
            );
        }
        if (index >= last_index) break;
    }
}

void SceneSelectSaveDataState::start_game() {
    owner_.select_save_data_index() = select_index_;
    owner_.change_state((GSuint)SceneStateType::End);
}

void SceneSelectSaveDataState::return_state() {
    owner_.change_state((GSuint)SceneStateType::MenuScene);
}
