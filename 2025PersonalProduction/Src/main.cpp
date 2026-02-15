// -----------------------------------------------------------------------------------------
//  File          : main.cpp
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/09
//  Description   : アプリケーションとして動作させるための機能を持つクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>
#include "GameConfig.h"
#include "Engine/Core/Assets/AssetsManager.h"
#include "Engine/Utils/OpenBrowser.h"
#include "Engine/Core/Screen/Screen.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Tween/Tween.h"
#include "Engine/Core/Setting/Setting.h"
#include "Engine/Utils/MyRandom.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "Engine/Core/LogSystem/LogManager.h"
#include "Engine/Core/Vibration/Vibration.h"
#include "Engine/Sound/SE.h"

#include "Engine/Core/Scene/SceneManager.h"
#include "Scene/LoadingScene.h"
#include "Scene/TitleScene.h"
#include "Scene/MenuScene.h"
#include "Scene/GameScene.h"

// tmp
#include "Scene/TimelineEditorScene.h"

class MyGame : public gslib::Game {
public:
    /// <summary>
    /// アプリケーションコンストラクタ
    /// </summary>
    /// <param name="width_px">= ウィンドウの初期幅(ピクセル)</param>
    /// <param name="height_px">= ウィンドウの初期高さ(ピクセル)</param>
    /// <param name="full_screen">= フルスクリーンにするなら真</param>
    /// <param name="refresh_rate">= アプリケーションのリフレッシュレート</param>
    MyGame(int width_px, int height_px, bool full_screen, float refresh_rate) : gslib::Game{ width_px, height_px, full_screen, refresh_rate } {
        // スクリーンデータをセット
        screen_.set_initialize_data(width_px, height_px, full_screen, refresh_rate);
        LogManager::get_instance().app_start();

        // 初期化
        gsInitEffect();
        scene_manager_.init();
        Canvas::init();
        SE::init();
        Setting::get_instance().load("tmp");
        MyRandom::set_seed(5);

#ifndef _DEBUG
        // マウスカーソルを非表示
        gsHideMouseCursor();
#endif
        // 初期音量調整
        gsSetPrimaryVolume(0.75f);

        // フォントを読み込む
        AddFontResourceExA("Resource/Assets/Font/DelaSukoGothicOne-R.ttf", FR_PRIVATE, NULL);
        AddFontResourceExA("Resource/Assets/Font/HackGen35Console-Regular.ttf", FR_PRIVATE, NULL);
    }

private:
    void start() override {
#ifdef _DEBUG
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("Resource/Assets/Font/HackGen35Console-Regular.ttf", 16.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
#endif

        // シーンを追加
        scene_manager_.add(new LoadingScene{});
        scene_manager_.add(new TitleScene{});
        scene_manager_.add(new MenuScene{});
        scene_manager_.add(new GameScene{});

        // tmp
        scene_manager_.add(new TimelineEditorScene{});

        // タイトルシーンから始める
        scene_manager_.change(SceneTag::Title);
    }

    void update(float delta_time) override {
        screen_.update();
        input_.update(delta_time);
        vibration_.update(delta_time);
        scene_manager_.update(delta_time);
    }

    void draw() override {
        scene_manager_.draw();
    }

    void end() override {
        // カーソルの移動制限を解除
        ClipCursor(NULL);
        // カーソルを表示
        gsShowMouseCursor();
        // コントローラーの振動を停止
        vibration_.end();

        // ログを出力
        LogManager::get_instance().save("Log/");

        // シーン終了
        scene_manager_.clear();

        // フォントを破棄
        RemoveFontResourceExA("Resource/Assets/Font/DelaSukoGothicOne-R.ttf", FR_PRIVATE, NULL);
        RemoveFontResourceExA("Resource/Assets/Font/HackGen35Console-Regular.ttf", FR_PRIVATE, NULL);
        // Tweenの破棄
        Tween::clear();
        // シェーダーの破棄
        GameShader::get_instance().clear();
        GamePostEffect::get_instance().clear();
        // アセットの破棄
        AssetsManager::get_instance().clear();
        SE::clear();
        SE::end();
        // エフェクトの終了
        gsFinishEffect();

//#ifndef _DEBUG
//        // アンケートフォームを開く
//        MyLib::open_browser_by_url("https://forms.gle/RvXri1WB3r3DGJQm7");
//#endif
    }

    /// <summary>
    /// アプリケーションの実行が継続するかどうか
    /// </summary>
    /// <returns>実行を継続するなら真</returns>
    bool is_running() override {
        // 強制終了
        if (input_.action(InputAction::APP_ForceEnd)) return false;
        
        // シーンからの終了リクエスト
        if (scene_manager_.is_application_end()) return false;

        return true;
    }

private:
    // シーンマネージャー
    SceneManager& scene_manager_ = SceneManager::get_instance();
    // スクリーン
    Screen& screen_ = Screen::get_instance();
    // インプット
    Input& input_ = Input::get_instance();
    // コントローラ－の振動
    Vibration& vibration_ = Vibration::get_instance();

};

int main() {
#ifdef _DEBUG
    return MyGame(1920, 1080, false, cFPS).run();
#else
    return MyGame(1920, 1080, true, cFPS).run();
#endif
}
