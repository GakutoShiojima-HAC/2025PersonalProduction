//  File        : World.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/25
//  Updated     : 2025/07/10
//  Description : ワールド管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef WORLD_H_
#define WORLD_H_

#include "Engine/Core/World/IWorld.h"
#include <gslib.h>
#include "Engine/Core/Actor/ActorManager.h"
#include "Engine/Core/Camera/CameraManager.h"

class World : public IWorld {
public:
	World() = default;

	virtual ~World();

public:
	virtual void update(float delta_time);

	virtual void draw() const;

	virtual void clear();

public:
	/// <summary>
	/// シャドウマップの描画用の関数
	/// </summary>
	static void shadow_map_callback(void* param, const GSmatrix4* view, const GSmatrix4* projection);

public:
	/// <summary>
	/// フィールドの追加
	/// </summary>
	/// <param name="field">= フィールド</param>
	void add_field(Field* field);

	/// <summary>
	/// ライトの追加
	/// </summary>
	/// <param name="light">= ライト</param>
	void add_light(Light* light);

	/// <summary>
	/// アクターの追加
	/// </summary>
	/// <param name="actor">アクター</param>
	void add_actor(Actor* actor);

public:
	Field* get_field() override;

	Light* get_light() override;
	
	void add_camera(Camera* camera) override;;

	Camera* get_camera() const override;

	Camera* find_camera(const CameraTag tag) const override;

	void camera_transition(Camera* to, float time = 0) override;

	void camera_transition(Camera* from, Camera* to, float time = 0) override;

	Actor* find_actor(const string& name) const override;

	vector<Actor*> find_actor_with_tag(const ActorTag tag) const override;
	
	int count_actor() const override;

	int count_actor_with_tag(const ActorTag tag) const override;

protected:
	// フィールド
	Field* field_{ nullptr };
	// ライト
	Light* light_{ nullptr };
	// アクターマネージャー
	ActorManager actor_;
	// カメラマネージャー
	CameraManager camera_;
};

#endif
