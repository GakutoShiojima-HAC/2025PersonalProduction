// -----------------------------------------------------------------------------------------
//  File          : ItemActor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/30
//  Updated       : 2025/09/30
//  Description   : アイテムクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ITEM_ACTOR_H_
#define ITEM_ACTOR_H_

#include "Engine/Core/Actor/Actor.h"
#include "Item/ItemData.h"

class ItemActor : public Actor {
public:
    ItemActor(IWorld* world, const GSvector3& position);

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="type">= アイテムの種類</param>
    /// <param name="id">= アイテムのID</param>
    ItemActor(IWorld* world, const GSvector3& position, ItemType type, GSuint id);

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="data">= アイテムデータ</param>
    ItemActor(IWorld* world, const GSvector3& position, ItemData::Data data);

    ~ItemActor();

public:
    virtual void update(float delta_time) override;

    virtual void draw() const override;

    virtual void die() override;

    virtual void message(const std::string& message, std::any& param)override;

    virtual bool can_interact() const override;

    virtual std::string interact_text() const override;

protected:
    /// <summary>
    /// 初期化
    /// </summary>
    void init();

    /// <summary>
    /// アイテムメッシュの更新
    /// </summary>
    void update_mesh(float delta_time);

protected:
    // アイテムデータ
    ItemData::Data data_{};
    // インタラクト時のテキスト
    std::string interact_text_{ "" };
    // アイテムアニメーションタイマー
    float animation_timer_{ 0.0f };
    // インタラクトされたかどうか
    bool is_interact_{ false };
    // エフェクトハンドル
    int effect_handle_{ 0 };

};

#endif
