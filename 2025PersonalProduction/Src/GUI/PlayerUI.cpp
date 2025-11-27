#include "PlayerUI.h"
#include <gslib.h>
#include <string>
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Assets.h"
#include "GameConfig.h"

void PlayerUI::draw_hp(int current, float display, int max) {
    const float max_hp = (float)max;

    // îwåiÇÃï`âÊ
    {
        const GSvector2 position{ 680.0, 972.0 };
        const GSrect pic_rect{ 0.0f, 0.0f, 500.0f, 25.0f };

        Canvas::draw_texture(
            (GSuint)TextureID::StatusGauge,
            position,
            pic_rect
        );
    }

    // ÉQÅ[ÉWÇÃï`âÊ
    {
        const GSvector2 position{ 743.0, 974.0 };

        // ëÃóÕÇÃäÑçá
        float ratio = display / max_hp;
        // Ç«Ç±Ç‹Ç≈êÿÇËéÊÇÈÇ©
        const GSrect pic_rect{ 0.0f, 0.0f, 434.0f * ratio, 21.0f };

        GScolor color;
        // ëÃóÕÇ™ç≈ëÂëÃóÕÇÃîºï™à»è„Ç»ÇÁóŒ
        if ((float)current > max_hp / 2.0f) color = GScolor{ 0.494f, 0.827f, 0.313f, 1.0f };
        // ëÃóÕÇ™ç≈ëÂëÃóÕÇÃ25Åìà»è„Ç»ÇÁâ©
        else if ((float)current > max_hp / 4.0f) color = GScolor{ 0.905f, 0.741f, 0.333f, 1.0f };
        // ÇªÇÍà»â∫ÇÕê‘
        else color = GScolor{ 0.772f, 0.192f, 0.192f, 1.0f };

        Canvas::draw_texture(
            (GSuint)TextureID::HPGauge,
            position,
            pic_rect,
            GSvector2{ 0.0f, 0.0f },
            GSvector2{ 1.0f, 1.0f },
            color
        );
    }

    // åªç›ëÃóÕÇÃï`âÊ
    {
        const GSvector2 position{ 6.0f, 972.0f };
        const std::string hp_str = to_string(current) + " /";
        Canvas::draw_sprite_text(
            hp_str,
            position,
            26,
            cFONT,
            GS_FONT_BOLD,
            GScolor{ 0.980f, 0.980f, 0.980f, 1.0f },
            Anchor::TopCenter,
            Anchor::TopRight
        );
    }

    // ç≈ëÂëÃóÕÇÃï`âÊ
    {
        const GSvector2 position{ 6.0f, 972.0f };
        const std::string hp_str = " " + to_string(max);
        Canvas::draw_sprite_text(
            hp_str,
            position,
            26,
            cFONT,
            GS_FONT_BOLD,
            GScolor{ 0.980f, 0.980f, 0.980f, 1.0f },
            Anchor::TopCenter,
            Anchor::TopLeft
        );
    }
    

}

void PlayerUI::draw_level(int level) {
    const GSvector2 position{ 705.0f, 985.0f };
    const std::string level_str = to_string(level);
    Canvas::draw_sprite_text(
        level_str,
        position,
        26,
        cFONT,
        GS_FONT_BOLD,
        GScolor{ 1.0f, 0.76f, 0.219f, 1.0f },
        Anchor::TopLeft,
        Anchor::Center
    );
}
