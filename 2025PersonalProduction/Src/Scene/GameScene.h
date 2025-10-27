// -----------------------------------------------------------------------------------------
//  File          : GameScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/10/20
//  Description   : ゲームシーン
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef GAME_SCENE_H_
#define GAME_SCENE_H_

#include "Engine/Core/Scene/StandardScene.h"
#include "Engine/Core/World/World.h"
#include "Engine/Core/Input/Input.h"
#include "Engine/Core/Actor/ActorGenerator.h"
#include "Stage/StageData.h"

class GameScene : public StandardScene {
public:
	GameScene();

	~GameScene() = default;

public:
    virtual void load() override;

    virtual void start() override;

    void update(float delta_time) override;

    void draw() const override;

	virtual void end() override;

    virtual SceneTag scene_tag() const override;

    virtual bool is_application_end() const override;

    virtual void reception_message(const std::string& message, std::any& param) override;

    void add_state() override;

    virtual void original_update(float delta_time) override;

    virtual void original_draw() const override;

public:
    /// <summary>
    /// ゲームGUIの描画を行うかどうか
    /// </summary>
    /// <returns>参照</returns>
    bool& enable_draw_game_gui();

protected:
    /// <summary>
    /// ゲーム用データのロード
    /// </summary>
    virtual void load_data();

    /// <summary>
    /// ゲームの開始処理
    /// </summary>
    virtual void game_start();

    /// <summary>
    /// ゲームの終了処理
    /// </summary>
    virtual void game_end();

protected:
	// ワールド
	World world_;
    // インプットシステム
    Input& input_ = Input::get_instance();
    // アクタージェネレーター
    ActorGenerator actor_generator_;
    // ステージデータ
    StageData stage_data_;

};

#endif
