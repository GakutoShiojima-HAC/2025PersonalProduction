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

class GameScene : public StandardScene {
public:
	GameScene() = default;

	~GameScene() = default;

public:
    virtual void load() override;

    virtual void start() override;

    virtual void update(float delta_time) override;

    virtual void draw() const override;

	virtual void end() override;

    virtual SceneTag scene_tag() const override;

    virtual bool is_application_end() const override;

    virtual void reception_message(const std::string& message, std::any& param) override;

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

};

#endif
