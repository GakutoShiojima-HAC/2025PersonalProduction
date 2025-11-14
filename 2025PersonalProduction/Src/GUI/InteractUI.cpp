#include "GUI/InteractUI.h"
#include "Engine/Graphics/Canvas/Canvas.h"
#include "Engine/Core/Actor/Actor.h"
#include "Engine/Utils/MyString.h"
// 何個UIを描画するか
const int INTERACT_LIST{ 4 };

void InteractUI::draw(const std::vector<Actor*>& actors, int target_index) {
    if (actors.empty()) return;

    // 先頭
    const int first_index = target_index > INTERACT_LIST ? target_index - INTERACT_LIST : 0;
    // 終端
    const int last_index = actors.size() - 1;

    for (size_t i = 0; i < INTERACT_LIST; ++i) {
        const int index = (int)i + first_index;
        if (index > last_index) break;
        const Actor* actor = actors[index];
        // テキストを取得
        std::string text = MyString::utf8_to_shiftjis((index == target_index ? "選択中: " : "")) + actor->interact_text();
        // テキストを描画
        Canvas::draw_text(
            text,                                       // テキスト
            GSvector2{ 1200.0f, 700.0f + (20.0f * i) },    // 描画位置
            18.0f                                       // フォントサイズ
        );
    }
}
