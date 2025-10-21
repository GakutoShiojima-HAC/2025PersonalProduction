// -----------------------------------------------------------------------------------------
//  File          : TimelineEditorScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/28
//  Updated       : 2025/10/20
//  Description   : タイムラインエディタデバッグ用シーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TIMELINE_EDITOR_SCENE_H_
#define TIMELINE_EDITOR_SCENE_H_

#include "Scene/GameScene.h"
#include "Engine/Core/Timeline/TimelineEditorManager.h"

class TimelineEditorScene : public GameScene {
public:
	TimelineEditorScene();

	~TimelineEditorScene() = default;

public:
	void start() override;

	void end() override;

	SceneTag scene_tag() const override;

	bool is_application_end() const override;

	void reception_message(const std::string& message, std::any& param) override;

    virtual void original_update(float delta_time) override;

    virtual void original_draw() const override;

private:
	// タイムラインエディタ
	TimelineEditorManager editor_;

};

#endif
