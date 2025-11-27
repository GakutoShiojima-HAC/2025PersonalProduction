#include "Scene/LoadingScene.h"
#include <gslib.h>
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Tween/Tween.h"
#include "Assets.h"
#include "Engine/Core/Screen/Screen.h"

void LoadingScene::start() {
    is_end_ = false;
    display_load_progress_ = 0.0f;

    // TODO loading image asset load

    // 次のシーンのロード処理を非同期で呼び出す
    gslib::Game::run_thread([=] {
        scene_manager_.load_scene(next_scene_tag_);
    });
}

void LoadingScene::update(float delta_time) {
    Tween::update(delta_time);

    // 描画と現在で差があるか
    const float current_load_progress = scene_manager_.load_progress(next_scene_tag_);
    if (std::fabs(display_load_progress_ - current_load_progress) > 0.0025f) {
        // 線形補間で合わせる
        display_load_progress_ += (current_load_progress - display_load_progress_) * 4.0f * delta_time / cFPS;
    }
    else {
        // 誤差対策
        display_load_progress_ = current_load_progress;
    }

    // 次のシーンのロード処理が終了したらシーンを終了する
    if (scene_manager_.is_load_end(next_scene_tag_) && display_load_progress_ >= 1.0f) is_end_ = true;
}

void LoadingScene::draw() const {
    // 背景描画
    ScreenData& data = Screen::get_instance().get_current_data();
    Canvas::draw_texture((GSuint)TextureID::BackGround, GSvector2::zero(), GSrect{ 0.0f, 0.0f, (float)data.width_px, (float)data.height_px });

    // 進捗率背景描画
    Canvas::draw_texture(
        (GSuint)TextureID::ProgressBarEmpty,
        GSvector2{ 208, 972 },
        GSrect{ 0.0f, 0.0f, 1504, 25 }
    );

    // 進捗率描画
    GSrect pic_rect{ 0.0f, 0.0f, 1500.0f, 21.0f };
    const GSvector2 position{ 210.0f, 974.0f };
    pic_rect.right *= display_load_progress_;
    Canvas::draw_texture(
        (GSuint)TextureID::ProgressBar,
        position,
        pic_rect
    );
}

void LoadingScene::end() {
    Tween::clear();

}

bool LoadingScene::is_end() const {
    return is_end_;
}

SceneTag LoadingScene::scene_tag() const {
    return SceneTag::Loading;
}

SceneTag LoadingScene::next_scene_tag() const {
    return next_scene_tag_;
}

bool LoadingScene::is_application_end() const {
    return false;
}

void LoadingScene::reception_message(const std::string& message, std::any& param) {
    if (message == "NextSceneTag" && param.type() == typeid(SceneTag)) {
        next_scene_tag_ = std::any_cast<SceneTag>(param);
    }
}

bool LoadingScene::is_load_end() const {
    return true;
}

float LoadingScene::load_progress() const {
    return 1.0f;
}
