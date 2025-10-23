// -----------------------------------------------------------------------------------------
//  File          : Actor.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : アクタークラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ACTOR_H_
#define ACTOR_H_

#include <GStransform.h>
#include <string>
#include <any>
#include "Actor/ActorTag.h"
#include "Engine/Core/Collision/BoundingSphere.h"
#include "GameConfig.h"

using namespace std;

class IWorld;

class Actor {
public:
	Actor() = default;

	virtual ~Actor() = default;

public:
	/// <summary>
	/// 更新
	/// </summary>
	virtual void update(float delta_time);

	/// <summary>
	/// 更新 updateとcollideの後に呼ばれる
	/// </summary>
	virtual void late_update(float delta_time);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void draw() const;

	/// <summary>
	/// 半透明描画
	/// </summary>
	virtual void draw_transparent() const;

	/// <summary>
	/// GUI描画
	/// </summary>
	virtual void draw_gui() const;

	/// <summary>
	/// アクターの寿命を終わらせる
	/// </summary>
	virtual void die();

	/// <summary>
	/// 他アクターと衝突したときに呼ばれる衝突処理衝突処理
	/// </summary>
	/// <param name="other">= 衝突したアクター</param>
	virtual void react(Actor& other);

public:
	/// <summary>
	/// 衝突判定
	/// </summary>
	void collide(Actor& other);

	/// <summary>
	/// アクターの寿命が尽きたかどうか
	/// </summary>
	bool is_dead() const;

	/// <summary>
	/// アクターの寿命が0だった場合メモリ解放処理まで行うかどうか
	/// </summary>
	/// <returns></returns>
	bool is_clear() const;

	/// <summary>
	/// 衝突しているかどうかを返却
	/// </summary>
	bool is_collide(const Actor& other) const;

	/// <summary>
	/// アクターの名前を返却
	/// </summary>
	string& name();

	/// <summary>
	/// アクターのタグを返却
	/// </summary>
	ActorTag& tag();

	/// <summary>
	/// トランスフォームを返却(const)
	/// </summary>
	const GStransform& transform() const;

	/// <summary>
	/// トランスフォームを返却
	/// </summary>
	GStransform& transform();

	/// <summary>
	/// 移動量を返却(const)
	/// </summary>
	GSvector3 velocity() const;

	/// <summary>
	/// 移動量を返却
	/// </summary>
	GSvector3& velocity();

	/// <summary>
	/// 衝突判定データを取得
	/// </summary>
	BoundingSphere collider() const;

	/// <summary>
	/// ローカル移動した後のワールド空間行列を返却
	/// </summary>
	/// <param name="position">= 移動量</param>
	/// <param name="rotate">= 回転量</param>
	/// <param name="scale">= 拡縮量</param>
	GSmatrix4 local_to_world(const GSvector3& position, const GSvector3& rotate, const GSvector3& scale) const;

	/// <summary>
	/// 衝突判定を行うかどうか
	/// </summary>
	/// <returns>行うなら真を返却</returns>
	bool is_collision() const;

	/// <summary>
	/// コライダーを描画
	/// </summary>
	void draw_collider() const;

    /// <summary>
    /// メッセージ
    /// </summary>
    /// <param name="message">= メッセージ識別文字列</param>
    /// <param name="param">= メッセージパラメータ</param>
    virtual void message(const std::string& message, std::any& param);

    /// <summary>
    /// アクターにインタラクトできるかどうか
    /// </summary>
    /// <returns>できるなら真を返却</returns>
    virtual bool can_interact() const;

    /// <summary>
    /// インタラクト時に表示する文字列を取得
    /// </summary>
    /// <returns>インタラクト時に表示する文字列</returns>
    virtual std::string interact_text() const;

protected:
	/// <summary>
	/// 地形との衝突判定
	/// </summary>
	virtual void collide_field();

	/// <summary>
	/// アクターとの衝突判定
	/// </summary>
	virtual void collide_actor(Actor& other);

	/// <summary>
	/// 地形を貫通しない移動
	/// </summary>
	/// <param name="velocity">= 移動量</param>
	/// <param name="foward">= 移動時に向く方向 指定無しで処理を行わない</param>
	/// <param name="trun_angle">= 移動時に向く方向への補間値</param>
	virtual void non_penetrating_move(const GSvector3& velocity, GSvector3* foward = nullptr, float trun_angle = 1.0f);

protected:
	/// <summary>
	/// エフェクシアのエフェクトを再生
	/// </summary>
	/// <param name="effect_id">= エフェクトハンドル</param>
	/// <param name="position">= ローカル移動量</param>
	/// <param name="rotation">= ローカル回転量</param>
	/// <param name="scale">= ローカル拡縮量</param>
	int play_effect(GSuint effect_id, const GSvector3& position, const GSvector3& rotate = GSvector3{ 0.0f, 0.0f, 0.0f }, const GSvector3& scale = GSvector3{ 1.0f, 1.0f, 1.0f }) const;

protected:
	// ワールド
	IWorld* world_{ nullptr };
	
	// 寿命
	bool is_dead_{ false };
	// 寿命が尽きたときメモリ解放まで行うか
	// ***危険*** オブジェクトプールでのみ使用すること
	bool is_clear_{ true };	

	// 名前
	string name_{ "Actor" };
	// タグ
	ActorTag tag_{ ActorTag::None };

	// トランスフォーム
	GStransform transform_;
	// 移動量
	GSvector3 velocity_{ 0.0f, 0.0f, 0.0f };

	// 衝突判定を行うかどうか
	bool enable_collider_{ true };
	// 球体衝突判定
	BoundingSphere collider_;

public:
	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

};

#endif
