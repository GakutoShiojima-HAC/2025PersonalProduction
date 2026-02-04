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
    /// 物理挙動制御
    /// </summary>
    enum class ForceMode {
        Force,          // 継続的
        Impulse,        // 瞬発的
        VelocityChange  // 質量無視
    };

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

#ifdef _DEBUG
    /// <summary>
    /// デバッグ用更新(DEBUGビルドのみ)
    /// </summary>
    virtual void debug_update(float delta_time);
#endif

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
    /// 接地状態かどうか
    /// </summary>
    /// <returns>接地状態なら真を返却</returns>
    bool is_grounded() const;

    /// <summary>
    /// 接地状態を更新
    /// </summary>
    /// <returns>参照</returns>
    bool& grounded();

    /// <summary>
    /// 重力を受けるかどうか
    /// </summary>
    /// <returns>参照</returns>
    bool& use_gravity();

    /// <summary>
    /// 重力を受けるかどうか(const)
    /// </summary>
    /// <returns>重力を受けるなら真を返却</returns>
    bool use_gravity() const;

    /// <summary>
    /// 重力値
    /// </summary>
    float gravity() const;

    /// <summary>
    /// 身長を取得
    /// </summary>
    float height() const;

    /// <summary>
    /// 力を加える
    /// </summary>
    /// <param name="force">= 量</param>
    /// <param name="mode">= 力の種類</param>
    virtual void add_force(const GSvector3& force, ForceMode mode);

	/// <summary>
	/// 衝突判定データを取得
	/// </summary>
	BoundingSphere collider() const;

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
	/// ローカル移動した後のワールド空間行列を返却
	/// </summary>
	/// <param name="position">= 移動量</param>
	/// <param name="rotate">= 回転量</param>
	/// <param name="scale">= 拡縮量</param>
	GSmatrix4 local_to_world(const GSvector3& position, const GSvector3& rotate, const GSvector3& scale) const;

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

    /// <summary>
    /// タイムスケールを受けるかどうか
    /// </summary>
    /// <returns>受けるなら真を返却</returns>
    bool is_enable_timescale() const;

    /// <summary>
    /// タイムスケールを有効にするかどうか
    /// </summary>
    /// <returns>参照</returns>
    bool& enable_timescale();

    /// <summary>
    /// 影をつけるかどうか
    /// </summary>
    /// <returns></returns>
    bool enabled_cast_shadow() const;

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
    /// 接地しなくなった瞬間
    /// </summary>
    virtual void on_air() {};

    /// <summary>
    /// 接地する瞬間
    /// </summary>
    virtual void on_ground() {};

	/// <summary>
	/// 地形を貫通しない移動
	/// </summary>
	/// <param name="velocity">= 移動量</param>
	/// <param name="foward">= 移動時に向く方向 指定無しで処理を行わない</param>
	/// <param name="trun_angle">= 移動時に向く方向への補間値</param>
	virtual void non_penetrating_move(const GSvector3& velocity, GSvector3* foward = nullptr, float trun_angle = 1.0f);

protected:
    /// <summary>
    /// 物理の更新
    /// </summary>
    void update_physics(float delta_time);

	/// <summary>
	/// エフェクシアのエフェクトを再生
	/// </summary>
	/// <param name="effect_id">= エフェクトハンドル</param>
	/// <param name="position">= ローカル移動量</param>
	/// <param name="rotation">= ローカル回転量</param>
	/// <param name="scale">= ローカル拡縮量</param>
    /// <param name="speed">= 再生速度</param>
	int play_effect(GSuint effect_id, const GSvector3& position, const GSvector3& rotate = GSvector3{ 0.0f, 0.0f, 0.0f }, const GSvector3& scale = GSvector3{ 1.0f, 1.0f, 1.0f }, float speed = 1.0f) const;

protected:
    // トランスフォーム
    GStransform transform_;
    // 移動量
    GSvector3 velocity_{ 0.0f, 0.0f, 0.0f };
    // 重力値
    float gravity_{ 0.981f };
    // 質量
    float mass_{ 1.0f };
    // 重力を受けるかどうか
    bool use_gravity_ = true;
    // 接地しているかどうか
    bool is_grounded_ = false;
    // 外部からの力を受けるかどうか
    bool use_force_external_ = true;

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

	// 衝突判定を行うかどうか
	bool enable_collider_{ true };
	// 球体衝突判定
	BoundingSphere collider_;
    // 身長
    float height_{ 2.0f };

    // タイムスケールを受けるかどうか
    bool enable_timescale_{ true };
    // 影を描画するかどうか
    bool enable_cast_shadow_{ true };

public:
	// コピー禁止
	Actor(const Actor& other) = delete;
	Actor& operator = (const Actor& other) = delete;

};

#endif
