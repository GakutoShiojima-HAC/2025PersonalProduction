#include "State/Scene/SceneMenuState.h"
#include "Engine/Core/Scene/StandardScene.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GUI/Button/TextFunctionButton.h"

SceneMenuState::SceneMenuState(StandardScene& owner) :
    owner_{ owner } {
    // É{É^ÉìÇÃí«â¡
    {
        TextFunctionButton* button = new TextFunctionButton{ "ç≈èâÇ©ÇÁ", GSvector2{ 960.0f, 679.0f }, 42, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.scene_end_request(); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "ë±Ç´Ç©ÇÁ(ñ¢é¿ëï)", GSvector2{ 960.0f, 765.0f }, 42, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.scene_end_request(); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "ÉQÅ[ÉÄê›íË", GSvector2{ 960.0f, 851.0f }, 42, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.change_state((GSuint)SceneStateType::Setting); });
        button_.add(button);
    }
    {
        TextFunctionButton* button = new TextFunctionButton{ "èIóπ", GSvector2{ 960.0f, 937.0f }, 42, Anchor::TopLeft, Anchor::Center };
        button->on_input([=] { owner_.app_end_request(); });
        button_.add(button);
    }
}

void SceneMenuState::enter() {
    button_.start();
}

void SceneMenuState::update(float delta_time) {
    button_.update(delta_time);
}

void SceneMenuState::draw() const {
    owner_.original_draw();

    // É{É^ÉìîwåiÇÃï`âÊ
    Canvas::draw_texture(
        (GSuint)TextureID::MenuWindowTexture,
        GSvector2{ 0.0f, 0.0f },
        GSrect{ 0.0f, 0.0f, 1920.0f, 1080.0f }
    );

    // âºÉçÉSï`âÊ    
    const GSrect logo_rect{ 0.0f, 0.0f, 340.0f, 76.0f };
    Canvas::draw_texture(
        (GSuint)TextureID::MenuLogo,
        GSvector2{ 0.0f, -200.0f },
        logo_rect,
        GSvector2{ logo_rect.right / 2.0f, logo_rect.bottom / 2.0f },
        GSvector2{ 1.0f, 1.0f },
        GScolor{ 1.0f, 1.0f, 1.0f, 1.0f },
        0.0f,
        Anchor::Center
    );

    button_.draw();
}
