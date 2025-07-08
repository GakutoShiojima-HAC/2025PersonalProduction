// -----------------------------------------------------------------------------------------
//  File        : main.cpp
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/08
//  Updated     : 2025/07/08
//  Description : アプリケーション動作用クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>

class MyGame : public gslib::Game {
public:
    /// <summary>
    /// アプリケーションコンストラクタ
    /// </summary>
    MyGame() : gslib::Game{ 640, 480, false, 60.0f } { }

private:
    /// <summary>
    /// アプリケーションの開始
    /// </summary>
    void start() override {

    }

    /// <summary>
    /// アプリケーションの更新
    /// </summary>
    void update(float delta_time) override {
    
    }

    /// <summary>
    /// アプリケーションの描画
    /// </summary>
    void draw() override {
    
    }

    /// <summary>
    /// アプリケーションの終了
    /// </summary>
    void end() override {
        // カーソルの移動制限を解除
        ClipCursor(NULL);
        // カーソルを表示
        gsShowMouseCursor();
    }

    /// <summary>
    /// アプリケーションの強制終了条件
    /// </summary>
    bool is_running() override {
        // TODO 終了条件
        
        return true;
    }
};

int main() {
    return MyGame().run();
}
