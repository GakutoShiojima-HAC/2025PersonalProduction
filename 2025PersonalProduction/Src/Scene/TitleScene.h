// -----------------------------------------------------------------------------------------
//  File          : TitleScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : タイトルシーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include "Engine/Core/Scene/IScene.h"
#include "Engine/Core/Scene/SceneManager.h"

class TitleScene : public IScene {
public:
	TitleScene() = default;

	~TitleScene() = default;

public:
    void load() override {};

	void start() override;
	
	void update(float delta_time) override;

	void draw() const override;

	void end() override;

	bool is_end() const override;

	SceneTag scene_tag() const override;

    SceneTag next_scene_tag() const override;

	bool is_application_end() const override;

	void reception_message(const std::string& message, std::any& param) override;

    bool is_load_end() const override;

    float load_progress() const override;

private:
    /// <summary>
    /// データロード用
    /// </summary>
    void load_data();

private:
    // シーンマネージャー
    SceneManager& scene_manager_ = SceneManager::get_instance();
    // シーンが終了したかどうか
    bool is_end_{ false };

};

#endif
