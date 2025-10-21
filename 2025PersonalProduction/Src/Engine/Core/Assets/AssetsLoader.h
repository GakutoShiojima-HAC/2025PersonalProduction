#ifndef ASSETS_LOADER_H_
#define ASSETS_LOADER_H_

#include <string>

namespace AssetsLoader {
#pragma region [シーン用アセット管理ID]
    // ゲーム共通アセットの識別名
    const std::string GENERAL_ASSET_NAME{ "GENERAL_ASSET" };
    // タイトルシーン用アセットの識別名
    const std::string TITLE_ASSET_NAME{ "TITLE_ASSET" };
    // メニューシーン用アセットの識別名
    const std::string MENU_ASSET_NAME{ "MENU_ASSET" };
    // ゲームシーン用共通アセットの識別名
    const std::string GAME_COMMON_ASSET_NAME{ "GAME_COMMON_ASSET" };
    // ゲームシーン用ステージ固有アセットの識別名
    const std::string GAME_STAGE_ASSET_NAME{ "GAME_STAGE_ASSET" };
#pragma endregion

    /// <summary>
    /// json形式で書かれたアセット読み込み定義ファイルから、アセットを全て読み込む
    /// </summary>
    /// <param name="file_path">= ファイルパス</param>
    /// <param name="name">= 読み込み済み構造体に付ける名前</param>
    /// <returns>成功なら真を返却</returns>
    bool load_by_json(const std::string& file_path, const std::string& name);
}

#endif
