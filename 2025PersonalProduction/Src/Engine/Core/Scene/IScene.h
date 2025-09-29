// -----------------------------------------------------------------------------------------
//  File          : IScene.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : シーンのインターフェース
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef I_SCENE_H_
#define I_SCENE_H_

#include <string>
#include <any>
#include "Scene/SceneTag.h"

class IScene {
public:
    IScene() = default;

    virtual ~IScene() = default;

public:
    /// <summary>
    /// シーンに必要なロード処理
    /// </summary>
    virtual void load() = 0;

    /// <summary>
    /// シーン開始
    /// </summary>
    virtual void start() = 0;

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="delta_time">= 前フレームからの経過フレーム数</param>
    virtual void update(float delta_time) = 0;

    /// <summary>
    /// 描画
    /// </summary>
    virtual void draw() const = 0;
    
    /// <summary>
    /// シーン終了
    /// </summary>
    virtual void end() = 0;

    /// <summary>
    /// シーンが終了しているか
    /// </summary>
    /// <returns>終了していたら真</returns>
    virtual bool is_end() const = 0;

    /// <summary>
    /// 現在のシーンタグを返却
    /// </summary>
    /// <returns>シーンタグ</returns>
    virtual SceneTag scene_tag() const = 0;
    
    /// <summary>
    /// 遷移先のシーンタグを返却
    /// </summary>
    /// <returns>シーンタグ</returns>
    virtual SceneTag next_scene_tag() const = 0;

    /// <summary>
    /// シーンがアプリケーション終了をリクエストしているか
    /// </summary>
    /// <returns>リクエストしていたら真</returns>
    virtual bool is_application_end() const = 0;

    /// <summary>
    /// 他のシーンからメッセージを受け取る
    /// </summary>
    /// <param name="message">= メッセージ識別文字列</param>
    /// <param name="param">= メッセージパラメータ</param>
    virtual void reception_message(const std::string& message, std::any& param) = 0;

    /// <summary>
    /// ロード処理が終了したかどうか
    /// </summary>
    /// <returns>終了したら真を返却</returns>
    virtual bool is_load_end() const = 0;

    /// <summary>
    /// ロード処理の進捗率を取得
    /// </summary>
    /// <returns>進捗率を0.0~1.0で返却</returns>
    virtual float load_progress() const = 0;

public:
    // コピー禁止
    IScene(const IScene& other) = delete;
    IScene& operator = (const IScene& other) = delete;

};

#endif
