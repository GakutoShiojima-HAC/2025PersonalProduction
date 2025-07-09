// -----------------------------------------------------------------------------------------
//  File          : GameScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : ゲームシーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "Engine/Core/Scene/IScene.h"

class GameScene : public IScene {
public:
	GameScene() = default;

	~GameScene() = default;

public:
	void start() override;

	void update(float delta_time) override;

	void draw() const override;

	void end() override;

	bool is_end() const override;

	SceneTag scene_tag() const override;

	SceneTag next_scene_tag() const override;

	bool is_application_end() const override;

	void reception_message(const std::string& message, void* param) override;

private:


private:
	bool is_end_{ false };

};

#endif
