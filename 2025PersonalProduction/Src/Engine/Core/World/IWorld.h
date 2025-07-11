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

public:
	// コピー禁止
	IWorld(const IWorld& other) = delete;
	IWorld& operator = (const IWorld& other) = delete;

};

#endif
