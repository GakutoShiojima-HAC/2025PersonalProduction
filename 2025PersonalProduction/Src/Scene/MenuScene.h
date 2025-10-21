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

	bool is_application_end() const override;

	void reception_message(const std::string& message, std::any& param) override;

    void add_state() override;

    void original_update(float delta_time) override;

    void original_draw() const override;

private:
    /// <summary>
    /// データロード用
    /// </summary>
    void load_data();

};

#endif
