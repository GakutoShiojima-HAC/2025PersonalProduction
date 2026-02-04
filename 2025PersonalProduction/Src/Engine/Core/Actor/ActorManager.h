// -----------------------------------------------------------------------------------------
//  File          : ActorManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : アクター管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ACTOR_MANAGER_H_
#define ACTOR_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Actor/ActorTag.h"

class Actor;

class ActorManager {
public:
	ActorManager() = default;

	virtual ~ActorManager();

public:
    /// <summary>
    /// アクターを管理下に追加
    /// </summary>
    /// <param name="actor">= アクター</param>
    void add(Actor* actor);

    /// <summary>
    /// アクターの更新
    /// </summary>
    virtual void update(float delta_time, float scale_time);

    /// <summary>
    /// アクターの更新 updateとcollideの後に呼ばれる
    /// </summary>
    void late_update(float delta_time, float scale_time);

    /// <summary>
    /// アクターの描画
    /// </summary>
    void draw() const;

    /// <summary>
    /// アクターの影用描画
    /// </summary>
    void draw_shadow() const;

    /// <summary>
    /// アクターの半透明描画
    /// </summary>
    void draw_tranparent() const;

    /// <summary>
    /// アクターのGUI描画
    /// </summary>
    void draw_gui() const;

    /// <summary>
    /// アクターの衝突判定
    /// </summary>
    void collide();

    /// <summary>
    /// 寿命が尽きたアクターの削除
    /// </summary>
    void remove();

    /// <summary>
    /// 全てのアクターを返却
    /// </summary>
    /// <returns>全てのアクター</returns>
    std::vector<Actor*> get_all() const;

    /// <summary>
    /// 指定した名前を持つアクターを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればアクターを、見つからなければnullptr</returns>
    Actor* find(const string& name) const;

    /// <summary>
    /// 指定したタグを持つアクターを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>アクター</returns>
    std::vector<Actor*> find_with_tag(const ActorTag tag) const;

    /// <summary>
    /// 管理しているアクターの数
    /// </summary>
    /// <returns>数</returns>
    int count() const;

    /// <summary>
    /// 指定したタグを持つアクターの数
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>数</returns>
    int count_with_tag(const ActorTag tag) const;

    /// <summary>
    /// 管理しているアクターを消去
    /// </summary>
    void clear();

protected:
    // アクターリスト
    std::list<Actor*> actors_;

#ifdef _DEBUG
    // コライダーを描画するかどうか
    bool draw_collision_detection_{ false };
    // アクターパラメータを描画するかどうか
    bool draw_actor_parameter_{ false };
    // どのアクターのデバッグ画面を描画するか
    int debug_actor_index_{ 0 };
#endif

public:
    // コピー禁止
    ActorManager(const ActorManager& other) = delete;
    ActorManager& operator = (const ActorManager& other) = delete;

};

#endif
