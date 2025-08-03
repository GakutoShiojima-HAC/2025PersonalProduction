// -----------------------------------------------------------------------------------------
//  File          : TimelineEditorScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/07/28
//  Description   : タイムラインエディタデバッグ用シーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_EDITOR_SCENE_H_
#define TIMELINE_EDITOR_SCENE_H_

#include "Engine/Core/Scene/IScene.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Timeline/TimelineEditorManager.h"

class TimelineEditorScene : public IScene {
public:
	TimelineEditorScene() = default;

	~TimelineEditorScene() = default;

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
	bool is_end_{ false };

	// ワールド
	World world_;
	// タイムラインエディタ
	TimelineEditorManager editor_;

};

#endif
