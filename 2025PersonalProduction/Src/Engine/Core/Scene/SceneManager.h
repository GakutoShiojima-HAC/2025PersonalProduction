// -----------------------------------------------------------------------------------------
//  File          : SceneManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/08
//  Updated       : 2025/07/08
//  Description   : シーン管理クラス
//                  シングルトンにしている理由はシーン間のデータ共有や、
//                  シーンマネージャーを複数実装されたくないため
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include <vector>
#include <string>
#include "Scene/SceneTag.h"

using namespace std;

class IScene;

class SceneManager {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    SceneManager() = default;

public:
    ~SceneManager();

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static SceneManager& get_instance();

public:
    /// <summary>
    /// シーンマネージャーの初期化
    /// </summary>
    void init();

    /// <summary>
    /// 現在のシーンを更新
    /// </summary>
    /// <param name="delta_time">= 前フレームからの経過フレーム数</param>
    void update(float delta_time);

    /// <summary>
    /// 現在のシーンを描画
    /// </summary>
    void draw();

    /// <summary>
    /// 現在のシーンを終了
    /// </summary>
    void end();

    /// <summary>
    /// 登録済みシーンを消去
    /// </summary>
    void clear();

public:
    /// <summary>
    /// シーンの追加
    /// </summary>
    /// <param name="scene">シーン</param>
    void add(IScene* scene);

    /// <summary>
    /// 指定したシーンタグのシーンに変更
    /// </summary>
    /// <param name="tag">= シーンタグ</param>
    void change(SceneTag tag);

    /// <summary>
    /// 管理中のシーンを全て返却
    /// </summary>
    /// <returns>全てのシーン</returns>
    vector<IScene*> get_scenes() const;

    /// <summary>
    /// 管理中のシーンの数を返却
    /// </summary>
    /// <returns>シーンの数</returns>
    int count() const;

    /// <summary>
    /// 現在のシーンを返却
    /// </summary>
    /// <returns>シーン</returns>
    IScene* get_current_scene() const;

    /// <summary>
    /// 現在のシーンタグを返却
    /// </summary>
    /// <returns>シーンタグ</returns>
    SceneTag get_current_scene_tag() const;

    /// <summary>
    /// 指定したシーンを検索
    /// </summary>
    /// <param name="tag">= シーンタグ</param>
    /// <returns>見つかればシーンを、見つからなければnullptr</returns>
    IScene* find(SceneTag tag) const;

    /// <summary>
    /// 現在のシーンがアプリケーション終了をリクエストしているか
    /// </summary>
    /// <returns>リクエストしていたら真</returns>
    bool is_application_end() const;

    /// <summary>
    /// 指定したシーンにメッセージを送る
    /// </summary>
    /// <param name="tag">= 送信先のシーンタグ</param>
    /// <param name="message">= メッセージ識別文字列</param>
    /// <param name="param">= メッセージパラメータ</param>
    void send_message(SceneTag tag, const string& message, void* param);

private:
    // 登録済みのシーン
    vector<IScene*> scenes_;
    // 現在のシーン
    IScene* current_scene_{ nullptr };

public:
	// コピー禁止
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;

};

#endif
