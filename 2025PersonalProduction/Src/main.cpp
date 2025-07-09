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
#include "Engine/Core/Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/MenuScene.h"
#include "Scene/GameScene.h"
#include "Engine/Utils/OpenBrowser.h"
#include "Engine/Core/Screen/Screen.h"
#include "Engine/Graphics/Canvas/Canvas.h"

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

        // 初期化
        scene_manager_.init();
        Canvas::init();
    }

private:
    void start() override {
        // シーンを追加
        scene_manager_.add(new TitleScene{});
        scene_manager_.add(new MenuScene{});
        scene_manager_.add(new GameScene{});

        // タイトルシーンから始める
        scene_manager_.change(SceneTag::Title);
    }

    void update(float delta_time) override {
        screen_.update();

        scene_manager_.update(delta_time);
    }

    void draw() override {
        scene_manager_.draw();
    }

    void end() override {
        scene_manager_.clear();

        // カーソルの移動制限を解除
        ClipCursor(NULL);
        // カーソルを表示
        gsShowMouseCursor();

        // アンケートフォームを開く
#ifndef _DEBUG
        MyLib::open_browser_by_url("https://forms.gle/RvXri1WB3r3DGJQm7");
#endif
    }

    /// <summary>
    /// アプリケーションの実行が継続するかどうか
    /// </summary>
    /// <returns>実行を継続するなら真</returns>
    bool is_running() override {
        // TODO 強制終了条件
        
        // シーンからの終了リクエスト
        if (scene_manager_.is_application_end()) return false;

        return true;
    }

private:
    // シーンマネージャー
    SceneManager& scene_manager_ = SceneManager::get_instance();
    // スクリーン
    Screen& screen_ = Screen::get_instance();

};

int main() {
#ifdef _DEBUG
    return MyGame(1920, 1080, false, 60.0f).run();
#else
    return MyGame(1920, 1080, true, 60.0f).run();
#endif
}
