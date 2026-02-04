#include "TutorialFacilitatorAvoidInputState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"

// テロップを見なければいけない時間
constexpr float ENABLE_INPUT_TIME{ 1.0f };

TutorialFacilitatorAvoidInputState::TutorialFacilitatorAvoidInputState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorAvoidInputState::enter() {
    is_show_ = false;
    timer_ = 0.0f;
}

void TutorialFacilitatorAvoidInputState::update(float delta_time) {
    // 攻撃態勢になったときテロップを表示する
    if (!is_show_ && owner_.is_attack_soon()) {
        is_show_ = true;

        // 拘束
        Input& input = Input::get_instance();
        input.disable_action(InputAction::GAME_Camera) = true;
        input.disable_action(InputAction::GAME_Jump) = true;
        input.disable_action(InputAction::GAME_Skill) = true;
        input.disable_action(InputAction::GAME_Lockon) = true;
        input.disable_action(InputAction::GAME_Move) = true;
        input.disable_action(InputAction::GAME_Attack) = true;

        input.disable_action(InputAction::GAME_Avoid) = true;
        is_avoid_disable_ = true;

        // 動かなくなる
        owner_.freeze_motion() = true;
        owner_.target()->freeze_motion() = true;
    }

    // タイマー
    if (is_show_) {
        timer_ += delta_time / cFPS;
    }

    // 回避入力拘束の解除
    if (is_avoid_disable_ && timer_ >= ENABLE_INPUT_TIME) {
        is_avoid_disable_ = false;
        Input& input = Input::get_instance();
        input.disable_action(InputAction::GAME_Avoid) = false;
    }

    // 回避を押したら
    if (Input::get_instance().action(InputAction::GAME_Avoid)) {
        // 次のテロップステートへ
        owner_.change_state((GSuint)TutorialFacilitatorStateType::AvoidAttackInput);
        return;
    }
}

void TutorialFacilitatorAvoidInputState::draw_gui() const {
    if (!is_show_) return;
    Canvas::draw_texture((GSuint)TextureID::TutorialEAttackTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    // 回避方法描画
    if (timer_ >= ENABLE_INPUT_TIME) {
        Canvas::draw_texture((GSuint)TextureID::TutorialAvoidInputTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });
    }
}

void TutorialFacilitatorAvoidInputState::exit() {
    // 解放
    Input& input = Input::get_instance();
    input.reset_disable_action();

    owner_.freeze_motion() = false;
    owner_.target()->freeze_motion() = false;
}
