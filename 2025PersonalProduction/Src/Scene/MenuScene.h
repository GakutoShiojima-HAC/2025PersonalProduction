// -----------------------------------------------------------------------------------------
//  File          : MenuScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : メニューシーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MENU_SCENE_H_
#define MENU_SCENE_H_

#include "Engine/Core/Scene/StandardScene.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"

struct MenuSceneSaveDataInfo {
    std::string name{ "" };
    std::string time{ "" };
    int stage{ -1 };
    int level{ 1 };
};

class MenuScene : public StandardScene {
public:
	MenuScene();

	~MenuScene() = default;

public:
    void load() override;

	void start() override;

	void update(float delta_time) override;

	void draw() const override;

	void end() override;

	SceneTag scene_tag() const override;

	void reception_message(const std::string& message, std::any& param) override;

    void add_state() override;

    void original_update(float delta_time) override;

    void original_draw() const override;

public:
    /// <summary>
    /// セーブデータの情報を取得
    /// </summary>
    /// <returns></returns>
    const std::vector<MenuSceneSaveDataInfo>& get_save_data() const;

    /// <summary>
    /// セーブデータのインデックス
    /// </summary>
    /// <returns>参照</returns>
    int& select_save_data_index();

private:
    /// <summary>
    /// データロード用
    /// </summary>
    void load_data();

    /// <summary>
    /// セーブデータの情報を読み込む
    /// </summary>
    void load_save_data_info();

private:
    // ゲームポストエフェクト
    GamePostEffect& game_post_effect_ = GamePostEffect::get_instance();

    // セーブデータ情報
    std::vector<MenuSceneSaveDataInfo> save_data_info_;
    // どのセーブデータを選ぶか
    int select_save_data_index_{ -1 };
};

#endif
