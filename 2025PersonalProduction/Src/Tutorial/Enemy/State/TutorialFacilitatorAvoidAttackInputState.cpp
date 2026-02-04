#include "TutorialFacilitatorAvoidAttackInputState.h"
#include "Tutorial/Enemy/TutorialFacilitator.h"
#include "Assets.h"
#include "Engine/Graphics/Canvas/Canvas.h"

TutorialFacilitatorAvoidAttackInputState::TutorialFacilitatorAvoidAttackInputState(TutorialFacilitator& owner) :
	TutorialFacilitatorState(owner) {

}

void TutorialFacilitatorAvoidAttackInputState::enter() {
    is_in_avoid_effect_ = false;
    owner_.target()->freeze_motion() = false;
    owner_.pause_avoid_effect();
}

void TutorialFacilitatorAvoidAttackInputState::update(float delta_time) {
    if (!is_in_avoid_effect_ && owner_.is_avoid_effect()) {
        is_in_avoid_effect_ = true;

        // S‘©
        Input& input = Input::get_instance();
        input.disable_action(InputAction::GAME_Camera) = true;
        input.disable_action(InputAction::GAME_Jump) = true;
        input.disable_action(InputAction::GAME_Skill) = true;
        input.disable_action(InputAction::GAME_Lockon) = true;
        input.disable_action(InputAction::GAME_Move) = true;
        input.disable_action(InputAction::GAME_Avoid) = true;

        input.disable_action(InputAction::GAME_Attack) = true;
        is_attack_disable_ = true;
    }

    // ‰ñ”ğ‹óŠÔ‚É“ü‚ç‚È‚©‚Á‚½‚Æ‚«—p
    if (owner_.is_motion_end()) {
        owner_.change_state((GSuint)TutorialFacilitatorStateType::AvoidInputChase, TutorialFacilitatorMotion::Move, true);
        return;
    }

    if (!is_in_avoid_effect_) return;

    // UŒ‚S‘©‚Ì‰ğœ
    if (is_attack_disable_) {
        is_attack_disable_ = false;
        Input& input = Input::get_instance();
        input.disable_action(InputAction::GAME_Attack) = false;
        // “®‚©‚È‚­‚È‚é
        owner_.freeze_motion() = true;
        owner_.target()->freeze_motion() = true;
    }

    // w’èŠÔ‰æ‘œ‚ğŒ©‚½Œã‚ÉUŒ‚‚ğ‰Ÿ‚µ‚½‚ç
    if (Input::get_instance().action(InputAction::GAME_Attack)) {
        // ‰ñ”ğUŒ‚¬Œ÷‚Æ‚·‚é
        owner_.hp() = owner_.my_info().hp * 2; // ‘Ì—ÍƒŠƒZƒbƒg ‘½•ªó‚¯‚ê‚é
        owner_.change_state((GSuint)TutorialFacilitatorStateType::Attack);
        return;
    }
}

void TutorialFacilitatorAvoidAttackInputState::draw_gui() const {
    if (!is_in_avoid_effect_) return;

    // ‰æ‘œ•`‰æ
    Canvas::draw_texture((GSuint)TextureID::TutorialAvoidAttackTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });

    // UŒ‚•û–@•`‰æ
    Canvas::draw_texture((GSuint)TextureID::TutorialAttackInputTexture, GSvector2{ 0.0f, 0.0f }, GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f });
}

void TutorialFacilitatorAvoidAttackInputState::exit() {
    // ‰ğ•ú
    Input& input = Input::get_instance();
    input.reset_disable_action();

    owner_.freeze_motion() = false;
    owner_.target()->freeze_motion() = false;

    owner_.resume_avoid_effect();   // ÄŠJ
}
