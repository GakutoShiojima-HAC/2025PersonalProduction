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
#include "Engine/Core/Actor/Pawn/PawnManager.h"
#include "Engine/Core/Actor/Pawn/Character/CharacterManager.h"
#include "Engine/Core/Camera/CameraManager.h"
#include "Engine/Core/Timeline/TimelineManager.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "SaveData/GameSaveData.h"
#include "Score/ActionScore.h"

class AttackColliderPool;

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
	/// ナビメッシュの追加
	/// </summary>
	/// <param name="navmesh">= ナビメッシュ</param>
	void add_navmesh(NavMeshSurface* navmesh);

	/// <summary>
	/// アクターの追加
	/// </summary>
	/// <param name="actor">= アクター</param>
	void add_actor(Actor* actor);

	/// <summary>
	/// ポーンの追加
	/// </summary>
	/// <param name="pawn">= ポーン</param>
	void add_pawn(Pawn* pawn);

	/// <summary>
	/// キャラクターの追加
	/// </summary>
	/// <param name="character">= キャラクター</param>
	void add_character(Character* character);

	/// <summary>
	/// 攻撃判定プールの追加
	/// </summary>
	/// <param name="pool">= 攻撃判定プール</param>
	void add_attack_collider_pool(AttackColliderPool* pool);

public:
	Field* get_field() override;

	Light* get_light() override;
	
	void add_camera(Camera* camera) override;;

	Camera* get_camera() const override;

	Camera* find_camera(const CameraTag tag) const override;

	void camera_transition(Camera* to, float time = 0) override;

	void camera_transition(Camera* from, Camera* to, float time = 0) override;

	NavMeshSurface* navmesh() override;

    std::vector<Actor*> get_all_actor() const override;

	Actor* find_actor(const std::string& name) const override;

    std::vector<Actor*> find_actor_with_tag(const ActorTag tag) const override;
	
	int count_actor() const override;

	int count_actor_with_tag(const ActorTag tag) const override;

	Pawn* find_pawn(const std::string& name) const override;

    std::vector<Pawn*> find_pawn_with_tag(const ActorTag tag) const override;

	Character* find_character(const std::string& name) const override;

    std::vector<Character*> find_character_with_tag(const ActorTag tag) const override;

	TimelineManager& timeline() override;

	void generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan = 1.0f, float delay = 0.0f) override;

    void set_mask_color(const GScolor& color) override;

	bool& enable_avoid_effect() override;

    GameSaveData& game_save_data() override;

    ActionScore& action_score() override;

protected:
	// フィールド
	Field* field_{ nullptr };
	// ライト
	Light* light_{ nullptr };
	// ナビメッシュ
	NavMeshSurface* navmesh_{ nullptr };
	// アクターマネージャー
	ActorManager actor_;
	// ポーンマネージャー
	PawnManager pawn_;
	// キャラクターマネージャー
	CharacterManager character_;
	// カメラマネージャー
	CameraManager camera_;
	// タイムラインマネージャー
	TimelineManager timeline_;
	// 攻撃判定プール
	AttackColliderPool* attack_collider_pool_{ nullptr };
	// ゲームシェーダー
    GameShader& game_shader_ = GameShader::get_instance();
    // ゲームポストエフェクト
    GamePostEffect& game_post_effect_ = GamePostEffect::get_instance();
    // ゲームセーブデータ
    GameSaveData game_save_data_;
    // アクションスコア
    ActionScore action_score_;

};

#endif
