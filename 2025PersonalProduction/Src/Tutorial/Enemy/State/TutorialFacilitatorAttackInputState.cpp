#include "TutorialFacilitatorAttackInputState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"

// ƒeƒƒbƒv‚ðŒ©‚È‚¯‚ê‚Î‚¢‚¯‚È‚¢ŽžŠÔ
constexpr float ENABLE_INPUT_TIME{ 1.0f };

TutorialFacilitatorAttackInputState::TutorialFacilitatorAttackInputState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorAttackInputState::enter() {
    // S‘©
    Input& input = Input::get_instance();
    input.disable_action(InputAction::GAME_Camera) = true;
    input.disable_action(InputAction::GAME_Avoid) = true;
    input.disable_action(InputAction::GAME_Jump) = true;
    input.disable_action(InputAction::GAME_Skill) = true;
    input.disable_action(InputAction::GAME_Lockon) = true;
    input.disable_action(InputAction::GAME_Move) = true;

    input.disable_action(InputAction::GAME_Attack) = true;
    is_attack_disable_ = true;
}

void TutorialFacilitatorAttackInputState::update(float delta_time) {
    // UŒ‚S‘©‚Ì‰ðœ
    if (is_attack_disable_ && owner_.state_timer() >= ENABLE_INPUT_TIME * cFPS) {
        is_attack_disable_ = false;
        Input& input = Input::get_instance();
        input.disable_action(InputAction::GAME_Attack) = false;
    }

    // UŒ‚‚ð‰Ÿ‚µ‚½‚ç
    if (Input::get_instance().action(InputAction::GAME_Attack)) {
        // UŒ‚‚ðŽó‚¯‚É’Ç‚¢‚©‚¯‚Í‚¶‚ß‚é
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Chase, TutorialFacilitatorMotion::Move, true);
        return;
    }
}

void TutorialFacilitatorAttackInputState::draw_gui() const {
    // ‰æ‘œ•`‰æ
    Canvas::draw_texture((GSuint)TextureID::TutorialPAttackTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    // UŒ‚•û–@•`‰æ
    if (owner_.state_timer() >= ENABLE_INPUT_TIME * cFPS) {
        printf("%f\n", owner_.state_timer());
        Canvas::draw_texture((GSuint)TextureID::TutorialAttackInputTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });
    }
}

void TutorialFacilitatorAttackInputState::exit() {
    // ‰ð•ú
    Input& input = Input::get_instance();
    input.reset_disable_action();
}
