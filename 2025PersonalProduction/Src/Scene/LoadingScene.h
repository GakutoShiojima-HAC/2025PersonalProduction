// -----------------------------------------------------------------------------------------
//  File          : LoadingScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/29
//  Updated       : 2025/09/29
//  Description   : ロード用シーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LOADING_SCENE_H_
#define LOADING_SCENE_H_

#include "Engine/Core/Scene/IScene.h"
#include "Engine/Core/Scene/SceneManager.h"

class LoadingScene : public IScene {
public:
    LoadingScene() = default;

    ~LoadingScene() = default;

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
    // シーンマネージャー
    SceneManager& scene_manager_ = SceneManager::get_instance();
    // シーンが終了したかどうか
    bool is_end_{ false };
    // 次のシーン
    SceneTag next_scene_tag_{ SceneTag::Null };

};

#endif
