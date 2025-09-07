// -----------------------------------------------------------------------------------------
//  File        : IWorld.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/07/10
//  Description : ワールド管理クラスのインターフェース
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef I_WORLD_H_
#define I_WORLD_H_

#include <string>
#include <vector>
#include "Camera/CameraTag.h"
#include "Actor/ActorTag.h"

using namespace std;

class Field;
class FieldActor;
class Light;
class Camera;
class Actor;
class Pawn;
class Character;
class TimelineManager;
class NavMeshSurface;

class IWorld {
public:
    IWorld() = default;

	virtual ~IWorld() = default;

public:
	/// <summary>
	/// フィールドの取得
	/// </summary>
	/// <returns>フィールド</returns>
	virtual Field* get_field() = 0;

	/// <summary>
	/// ライトの取得
	/// </summary>
	/// <returns>ライト</returns>
	virtual Light* get_light() = 0;

    /// <summary>
    /// カメラを追加
    /// </summary>
    /// <param name="camera">カメラ</param>
    virtual void add_camera(Camera* camera) = 0;

    /// <summary>
    /// 現在使用しているカメラを取得
    /// </summary>
    /// <returns>カメラ</returns>
    virtual Camera* get_camera() const = 0;

    /// <summary>
    /// カメラを検索
    /// </summary>
    /// <param name="tag">= カメラタグ</param>
    /// <returns>見つかればカメラを、見つからなければnullptr</returns>
    virtual Camera* find_camera(const CameraTag tag) const = 0;

    /// <summary>
    /// 現在のカメラから指定したカメラに遷移
    /// </summary>
    /// <param name="to">= 遷移先</param>
    /// <param name="time">= 遷移にかかる時間</param>
    virtual void camera_transition(Camera* to, float time = 0) = 0;

    /// <summary>
    /// 指定したカメラから指定したカメラに遷移
    /// </summary>
    /// <param name="from">= 遷移元</param>
    /// <param name="to">= 遷移先</param>
    /// <param name="time">= 遷移にかかる時間</param>
    virtual void camera_transition(Camera* from, Camera* to, float time = 0) = 0;

    /// <summary>
    /// ナビメッシュを取得
    /// </summary>
    /// <returns>ナビメッシュ</returns>
    virtual NavMeshSurface* navmesh() = 0;

    /// <summary>
    /// 指定した名前を持つアクターを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればアクターを、見つからなければnullptr</returns>
    virtual Actor* find_actor(const string& name) const = 0;

    /// <summary>
    /// 指定したタグを持つアクターを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>アクター</returns>
    virtual vector<Actor*> find_actor_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// 管理しているアクターの数
    /// </summary>
    /// <returns>数</returns>
    virtual int count_actor() const = 0;

    /// <summary>
    /// 指定したタグを持つアクターの数
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>数</returns>
    virtual int count_actor_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// 指定した名前を持つポーンを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればポーンを、見つからなければnullptr</returns>
    virtual Pawn* find_pawn(const string& name) const = 0;

    /// <summary>
    /// 指定したタグを持つポーンを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>ポーン</returns>
    virtual vector<Pawn*> find_pawn_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// 指定した名前を持つキャラクターを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればキャラクターを、見つからなければnullptr</returns>
    virtual Character* find_character(const string& name) const = 0;

    /// <summary>
    /// 指定したタグを持つキャラクターを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>キャラクター</returns>
    virtual vector<Character*> find_character_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// タイムラインマネージャーを取得
    /// </summary>
    /// <returns>タイムラインマネージャー</returns>
    virtual TimelineManager& timeline() = 0;

    /// <summary>
    /// 攻撃判定を生成
    /// </summary>
    /// <param name="radius">= 判定の半径</param>
    /// <param name="center">= 生成判定球の中心</param>
    /// <param name="owner">= 生成主</param>
    /// <param name="damage">= ダメージ値</param>
    /// <param name="lifespan">= 判定の寿命</param>
    /// <param name="delay">= 判定が有効になるまでの遅延</param>
    virtual void generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, float lifespan = 1.0f, float delay = 0.0f) = 0;

    /// <summary>
    /// 回避演出用ポストエフェクトを描画するかどうか
    /// </summary>
    /// <returns>参照</returns>
    virtual bool& enable_avoid_posteffct() = 0;

    /// <summary>
    /// 回避演出用ポストエフェクトの色を設定
    /// </summary>
    /// <param name="color">RGB</param>
    virtual void set_avoid_effect_color(const GSvector3& color) = 0;

public:
	// コピー禁止
	IWorld(const IWorld& other) = delete;
	IWorld& operator = (const IWorld& other) = delete;

};

#endif
