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
	/// 移動量を返却
	/// </summary>
	GSvector3 velocity() const;

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

protected:
	/// <summary>
	/// エフェクシアのエフェクトを再生
	/// </summary>
	/// <param name="effect_id">= エフェクトハンドル</param>
	/// <param name="position">= ローカル移動量</param>
	/// <param name="rotation">= ローカル回転量</param>
	/// <param name="scale">= ローカル拡縮量</param>
	GSuint play_effect(GSuint effect_id, const GSvector3& position, const GSvector3& rotate = GSvector3{ 0.0f, 0.0f, 0.0f }, const GSvector3& scale = GSvector3{ 1.0f, 1.0f, 1.0f }) const;

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
