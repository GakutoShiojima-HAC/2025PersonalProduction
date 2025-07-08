// -----------------------------------------------------------------------------------------
//  File          : main.cpp
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : アプリケーションとして動作させるための機能を持つクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#include <GSgame.h>
#include <GSeffect.h>
#include <GSmovie.h>

class MyGame : public gslib::Game {
public:
    MyGame() : gslib::Game{ 640, 480, false, 60.0f } { }

private:
    void start() override {

    }

    void update(float delta_time) override {
    
    }

    void draw() override {
    
    }

    void end() override {
        // カーソルの移動制限を解除
        ClipCursor(NULL);
        // カーソルを表示
        gsShowMouseCursor();
    }

    /// <summary>
    /// アプリケーションの実行が継続するかどうか
    /// </summary>
    /// <returns>実行を継続するなら真</returns>
    bool is_running() override {
        // TODO 終了条件
        
        return true;
    }
};

int main() {
    return MyGame().run();
}
