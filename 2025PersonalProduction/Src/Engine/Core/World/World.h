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
#include "Engine/Core/Timeline/Timeline.h"
#include "Engine/Core/NavMesh/NavMeshSurface.h"
#include "Engine/Graphics/Shader/GameShader.h"
#include "Engine/Graphics/Shader/GamePostEffect.h"
#include "SaveData/GameSaveData.h"
#include "Score/ActionScore.h"
#include "GameTimer/GameTimer.h"
#include "Actor/Player/PlayerRespawner.h"
#include "Engine/Core/Field/StaticEffect.h"
#include "Engine/Graphics/TypeWriter/TypeWriterLabel.h"
#include "Engine/Core/World/AvoidEffectManager.h"
#include "Engine/Core//World/ImpactEffectManager.h"

class AttackColliderPool;

class World : public IWorld {
public:
	World();

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

    /// <summary>
    /// ゲーム時間を取得
    /// </summary>
    /// <returns>参照</returns>
    GameTimer& time();

    /// <summary>
    /// タイムラインを取得
    /// </summary>
    /// <returns>参照</returns>
    Timeline& timeline();

    /// <summary>
    /// プレイヤーリスポーンマネージャーを取得
    /// </summary>
    /// <returns>参照</returns>
    PlayerRespawner& player_respawner();

    /// <summary>
    /// 静的エフェクトを読み込む
    /// </summary>
    /// <param name="json_file">= データ</param>
    void load_static_effect(const std::string& json_file);

    /// <summary>
    /// 静的エフェクトを生成
    /// </summary>
    void generate_static_effect();

    /// <summary>
    /// 回避エフェクトを有効化するかどうか
    /// </summary>
    /// <returns>参照</returns>
    bool& enable_mask_avoid_effect();

public:
    void add_actor(Actor* actor) override;

    bool& enable_draw_gui() override;

    bool& enable_draw_game_info_gui() override;

	Field* get_field() override;

	Light* get_light() override;
	
	void add_camera(Camera* camera) override;;

	Camera* get_camera() const override;

	Camera* find_camera(const CameraTag tag) const override;

	void camera_transition(Camera* to, float time = 0) override;

	void camera_transition(Camera* from, Camera* to, float time = 0) override;

    void camera_shake(CameraShakeType type, float duration, float strength = 1.0f, bool loop = false) override;

    void camera_shake_end() override;

    bool& camera_enable_shake() override;

    int camera_effect_play_foward(GSuint id, float time) override;

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

    void play_timeline(const std::string& name, bool enable_timescale) override;

    void stop_timeline() override;

    bool is_playing_timeline() const override;

	void generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan = 1.0f, float delay = 0.0f, const GSvector3& external_velocity = GSvector3{ 0.0f, 0.0f, 0.0f }) override;

    void set_mask_color(const GScolor& color) override;

    bool is_draw_mask() const override;

    void start_avoid_effect(float time, float time_scale = -1.0f) override;

    void pause_avoid_effect(float time = -1.0f) override;

    void resume_avoid_effect() override;

    bool is_avoid_effect() const override;

    float& set_blur_effect_power() override;

    GameSaveData& game_save_data() override;

    ActionScore& action_score() override;

    float& timescale() override;

    void set_timescale(float scale = 1.0f, float time = 0.0f) override;

    void update_check_point(const GSvector3& position, const GSvector3& rotate) override;

    void skip_type_writer() override;

    void set_type_writer(const std::string& text, TextCode code = TextCode::UTF8) override;

    void set_type_writer(const std::vector<std::string>& text, TextCode code = TextCode::UTF8) override;

    void impact_effect_start(float time) override;

protected:
    // GUIを描画するかどうか
    bool enable_draw_gui_{ true };
    bool enable_draw_game_info_gui_{ true };

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
	// タイムライン
	Timeline timeline_;
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
    // ゲーム時間
    GameTimer game_timer_;
    // タイムスケール
    float timescale_{ 1.0f };
    // プレイヤーリスポーンマネージャー
    PlayerRespawner player_respawner_;
    // 静的エフェクトマネージャー
    StaticEffect static_effect_;
    // 文字送りシステム
    TypeWriterLabel type_writer_label_;
    // 回避エフェクトマネージャー
    AvoidEffectManager avoid_effect_;
    // インパクトエフェクトマネージャー
    ImpactEffectManager impact_effect_;
};

#endif
