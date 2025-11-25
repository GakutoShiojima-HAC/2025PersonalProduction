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
#include "Engine/Core/Camera/CameraShakeType.h"

class Field;
class FieldActor;
class Light;
class Camera;
class Actor;
class Pawn;
class Character;
class NavMeshSurface;
class GameSaveData;
class ActionScore;

struct GSvector3;

class IWorld {
public:
    IWorld() = default;

	virtual ~IWorld() = default;

public:
    /// <summary>
    /// アクターの追加
    /// </summary>
    /// <param name="actor">= アクター</param>
    virtual void add_actor(Actor* actor) = 0;

    /// <summary>
    /// GUIの描画を行うかどうか
    /// </summary>
    /// <returns>参照</returns>
    virtual bool& enable_draw_gui() = 0;

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
    /// カメラを揺らす
    /// </summary>
    /// <param name="type">= 揺れの種類</param>
    /// <param name="duration">= 揺らす時間(秒)</param>
    /// <param name="strength">= 揺れの強さ</param>
    /// <param name="loop">= ループするかどうか(時間を無視する)</param>
    virtual void camera_shake(CameraShakeType type, float duration, float strength = 1.0f, bool loop = false) = 0;

    /// <summary>
    /// 揺れを終了
    /// </summary>
    virtual void camera_shake_end() = 0;

    /// <summary>
    /// 揺れを有効にするかどうか
    /// </summary>
    /// <returns>参照</returns>
    virtual bool& camera_enable_shake() = 0;

    /// <summary>
    /// カメラの正面にエフェクトを再生する
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="time">= 時間(秒)</param>
    /// <returns>ハンドル</returns>
    virtual int camera_effect_play_foward(GSuint id, float time) = 0;

    /// <summary>
    /// ナビメッシュを取得
    /// </summary>
    /// <returns>ナビメッシュ</returns>
    virtual NavMeshSurface* navmesh() = 0;

    /// <summary>
    /// 全てのアクターを返却
    /// </summary>
    /// <returns>全てのアクター</returns>
    virtual std::vector<Actor*> get_all_actor() const = 0;

    /// <summary>
    /// 指定した名前を持つアクターを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればアクターを、見つからなければnullptr</returns>
    virtual Actor* find_actor(const std::string& name) const = 0;

    /// <summary>
    /// 指定したタグを持つアクターを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>アクター</returns>
    virtual std::vector<Actor*> find_actor_with_tag(const ActorTag tag) const = 0;

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
    virtual Pawn* find_pawn(const std::string& name) const = 0;

    /// <summary>
    /// 指定したタグを持つポーンを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>ポーン</returns>
    virtual std::vector<Pawn*> find_pawn_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// 指定した名前を持つキャラクターを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればキャラクターを、見つからなければnullptr</returns>
    virtual Character* find_character(const std::string& name) const = 0;

    /// <summary>
    /// 指定したタグを持つキャラクターを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>キャラクター</returns>
    virtual std::vector<Character*> find_character_with_tag(const ActorTag tag) const = 0;

    /// <summary>
    /// タイムラインの再生を開始
    /// </summary>
    /// <param name="name">= 再生時の名前</param>
    /// <param name="enable_timescale">= タイムスケールを受けるかどうか</param>
    virtual void play_timeline(const std::string& name, bool enable_timescale = false) = 0;

    /// <summary>
    /// タイムラインの再生を停止
    /// </summary>
    virtual void stop_timeline() = 0;

    /// <summary>
    /// タイムラインが再生中かどうか
    /// </summary>
    /// <returns></returns>
    virtual bool is_playing_timeline() const = 0;

    /// <summary>
    /// 攻撃判定を生成
    /// </summary>
    /// <param name="radius">= 判定の半径</param>
    /// <param name="center">= 生成判定球の中心</param>
    /// <param name="owner">= 生成主</param>
    /// <param name="damage">= ダメージ値</param>
    /// <param name="lifespan">= 判定の寿命</param>
    /// <param name="delay">= 判定が有効になるまでの遅延</param>
    virtual void generate_attack_collider(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan = 1.0f, float delay = 0.0f) = 0;

    /// <summary>
    /// マスクエフェクトの色を設定
    /// </summary>
    /// <param name="color">RGBA</param>
    virtual void set_mask_color(const GScolor& color) = 0;

    /// <summary>
    /// 回避演出用ポストエフェクトを描画するかどうか
    /// </summary>
    /// <returns>参照</returns>
    virtual bool& enable_avoid_effect() = 0;

    /// <summary>
    /// シーン描画をどれだけぼかすかどうかを設定(0.0~1.0)
    /// </summary>
    /// <returns>参照</returns>
    virtual float& set_blur_effect_power() = 0;

    /// <summary>
    /// ゲームセーブデータの取得
    /// </summary>
    /// <returns>ゲームセーブデータ管理クラス</returns>
    virtual GameSaveData& game_save_data() = 0;

    /// <summary>
    /// アクションスコアの取得
    /// </summary>
    /// <returns>参照</returns>
    virtual ActionScore& action_score() = 0;

    /// <summary>
    /// タイムスケールの取得
    /// </summary>
    /// <returns>参照</returns>
    virtual float& timescale() = 0;

    /// <summary>
    /// タイムスケールを変更
    /// </summary>
    /// <param name="scale">= スケール デフォルトは1.0</param>
    /// <param name="time">= 元のスケールから指定したスケールになるまでにかかる時間(delta_time)</param>
    virtual void set_timescale(float scale= 1.0f, float time = 0.0f) = 0;

    /// <summary>
    /// チェックポイントを更新
    /// </summary>
    /// <param name="position">= 復活時の座標</param>
    /// <param name="rotate">= 復活時の回転</param>
    virtual void update_check_point(const GSvector3& position, const GSvector3& rotate) = 0;

public:
	// コピー禁止
	IWorld(const IWorld& other) = delete;
	IWorld& operator = (const IWorld& other) = delete;

};

#endif
