// -----------------------------------------------------------------------------------------
//  File          : StandardScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/29
//  Updated       : 2025/09/29
//  Description   : 実際に使うシーンの基礎 ロードシーンを挟むことを前提としている
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef STANDARD_SCENE_H_
#define STANDARD_SCENE_H_

#include "Engine/Core/Scene/StandardScene.h"
#include "Engine/Core/Scene/SceneManager.h"
#include "Engine/Core/StateMachine/StateMachine.h"

class StandardScene : public IScene {
public:
    StandardScene() = default;

    ~StandardScene() = default;

public:
    /// <summary>
    /// ステートの変更
    /// </summary>
    /// <param name="state_num">= ステート番号</param>
    void change_state(const GSuint state_num);

protected:
    /// <summary>
    /// ステートの追加
    /// </summary>
    virtual void add_state() = 0;

public:
    virtual void load() override {}

    virtual void start() override {}

    virtual void update(float delta_time) override {}

    virtual void draw() const override {}

    virtual void end() override {}

    virtual bool is_end() const override;

    virtual SceneTag scene_tag() const override;

    virtual SceneTag next_scene_tag() const override;

    virtual bool is_application_end() const override;

    virtual void reception_message(const std::string& message, std::any& param) override {}

    virtual bool is_load_end() const override;

    virtual float load_progress() const override;

public:
    /// <summary>
    /// シーン独自の更新
    /// </summary>
    virtual void original_update(float delta_time) = 0;

    /// <summary>
    /// シーン独自の描画
    /// </summary>
    virtual void original_draw() const = 0;

protected:
    // シーンマネージャー
    SceneManager& scene_manager_ = SceneManager::get_instance();
    // シーンが終了したかどうか
    bool is_end_{ false };
    // 次のシーン
    SceneTag next_scene_tag_{ SceneTag::Null };
    // ロード処理が終了したかどうか
    bool is_load_end_{ false };
    // ロード処理の進捗率
    float load_progress_{ 0.0f };
    // ステートマシン
    StateMachine state_;

};

#endif
