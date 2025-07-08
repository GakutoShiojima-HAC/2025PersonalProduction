// -----------------------------------------------------------------------------------------
//  File          : AnimatedMesh.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : スキニングメッシュをアニメーションさせるクラス
//					アニメーションイベント対応
//					アニメーションレイヤー対応
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include <gslib.h>
#include <vector>
#include <functional>
#include <vector>
#include <memory>

using namespace std;

class GStransform;

struct AnimationEvent {
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="motion">= イベントを発火する対象のモーション番号</param>
	/// <param name="time">= イベントを発火する時間</param>
	/// <param name="callback">= イベント処理</param>
	AnimationEvent(GSuint motion, GSfloat time, std::function<void()> callback) :
		motion_{ motion },
		time_{ time },
		callback_{ callback } {
	}
	// イベントを発生させるモーション番号
	GSuint motion_;
	// イベントを発生させるタイミング
	GSfloat time_;
	// イベント発生時のコールバック
	std::function<void()> callback_;

};

class AnimatedMesh {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mesh">= メッシュ番号</param>
	/// <param name="skeleton">= スケルトン番号</param>
	/// <param name="animation">= アニメーション番号</param>
	/// <param name="motion">= 初期状態のモーション番号</param>
	/// <param name="loop">= 初期状態のモーションをループするかどうか</param>
	/// <param name="layer_count">= アニメーションレイヤー数</param>
	/// <param name="bone_count">= 使用しているボーンの数</param>
	AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion = 0, bool loop = true, GSuint layer_count = 1, GSuint bone_count = 256);
	
	~AnimatedMesh() = default;

public:
	void update(float delta_time);

	void draw() const;

public:
	/// <summary>
	/// モーションの変更（レイヤー指定なし）
	/// </summary>
	/// <param name="motion">= モーション番号</param>
	/// <param name="loop">= ループするかどうか</param>
	void change_motion(GSuint motion, bool loop = true);

	/// <summary>
	/// モーションの変更（レイヤー指定あり）
	/// </summary>
	/// <param name="layer">= レイヤー番号</param>
	/// <param name="motion">= モーション番号</param>
	/// <param name="loop">= ループするかどうか</param>
	void change_motion(GSuint layer, GSuint motion, bool loop = true);
	
	/// <summary>
	/// レイヤーインデックスの設定
	/// </summary>
	/// <param name="layer_index">= レイヤー番号</param>
	/// <param name="layer_bones">= レイヤーが扱うボーン番号</param>
	/// <param name="layer_bones_count">= レイヤーが扱うボーン番号の数</param>
	void set_layer_indices(GSuint layer_index, const GSuint layer_bones[], GSuint layer_bones_count);

	/// <summary>
	/// 変換行列を設定する
	/// </summary>
	/// <param name="matrix">= ワールド変換行列</param>
	void transform(const GSmatrix4& matrix);

	/// <summary>
	/// 現在再生しているモーションの終了時間を取得
	/// </summary>
	/// <param name="layer">= レイヤー番号</param>
	/// <returns>終了時間</returns>
	float motion_end_time(GSuint layer = 0) const;

	/// <summary>
	/// モーション再生が終了しているか
	/// </summary>
	/// <param name="layer">= レイヤー番号</param>
	/// <returns>終了していたら真、ループなら偽</returns>
	bool is_motion_end(GSuint layer = 0) const;

	/// <summary>
	/// ボーンのワールド変換行列を取得
	/// </summary>
	/// <param name="bone_no">= ボーン番号</param>
	/// <returns>ワールド変換行列</returns>
	GSmatrix4 bone_matrices(int bone_no) const;
	
	/// <summary>
	/// ボーンのローカル変換行列を取得
	/// </summary>
	/// <param name="bone_no">= ボーン番号</param>
	/// <returns>ローカル変換行列</returns>
	GSmatrix4 bone_local_matrices(int bone_no) const;
	
	/// <summary>
	/// 現在のモーションの総再生時間を取得
	/// </summary>
	/// <param name="layer">= レイヤー番号</param>
	/// <returns>総再生時間</returns>
	float current_motion_time(GSuint layer = 0) const;
	
	/// <summary>
	/// 現在のモーションの再生時間を設定
	/// </summary>
	/// <param name="time">= 再生したい時間</param>
	/// <param name="layer">= レイヤー番号</param>
	void current_motion_time(float time, GSuint layer = 0);

	/// <summary>
	/// ルートモーションの適用
	/// </summary>
	/// <param name="transform">= トランスフォーム</param>
	void apply_root_motion(GStransform& transform);

	/// <summary>
	/// アニメーションイベントを追加（レイヤー指定なし）
	/// </summary>
	/// <param name="motion">= イベントを発火する対象のモーション番号</param>
	/// <param name="time">= イベントを発火する時間</param>
	/// <param name="callback">= イベント処理</param>
	void add_animation_event(GSuint motion, GSfloat time, function<void()> callback);

	/// <summary>
	/// アニメーションイベントを追加（レイヤー指定あり）
	/// </summary>
	/// <param name="layer">= レイヤー番号</param>
	/// <param name="motion">= イベントを発火する対象のモーション番号</param>
	/// <param name="time">= イベントを発火する時間</param>
	/// <param name="callback">= イベント処理</param>
	void add_animation_event(GSuint layer, GSuint motion, GSfloat time, function<void()> callback);

private:
	class Animation {
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="animation">= アニメーション番号</param>
		/// <param name="motion">= 初期状態のモーション番号</param>
		/// <param name="loop">= 初期状態のモーションをループするかどうか</param>
		Animation(GSuint animation = 0, GSuint motion = 0, bool loop = true);

	public:
		void update(float delta_time);

	public:
		/// <summary>
		/// モーションの変更
		/// </summary>
		/// <param name="motion">= モーション番号</param>
		/// <param name="loop">= ループするかどうか</param>
		void change_motion(GSuint motion, bool loop = true);

		/// <summary>
		/// 再生中のモーション番号
		/// </summary>
		/// <returns>モーション番号</returns>
		GSuint motion_num() const;

		/// <summary>
		/// 現在再生しているモーションの終了時間を取得
		/// </summary>
		/// <returns>終了時間</returns>
		float motion_end_time() const;

		/// <summary>
		/// 現在のモーションの総再生時間を取得
		/// </summary>
		/// <returns>総再生時間</returns>
		float current_motion_time() const;

		/// <summary>
		/// 現在のモーションの再生時間を設定
		/// </summary>
		/// <param name="time">= 再生したい時間</param>
		void current_motion_time(float time);

		/// <summary>
		/// モーション再生が終了しているか
		/// </summary>
		/// <returns>終了していたら真、ループなら偽</returns>
		bool is_motion_end() const;

		/// <summary>
		/// ボーンのローカル変換行列を取得
		/// </summary>
		/// <param name="bone_no">= ボーン番号</param>
		/// <returns>ローカル変換行列</returns>
		GSmatrix4 local_bone_matrix(GSuint bone_no) const;

		/// <summary>
		/// ボーン数を取得
		/// </summary>
		/// <returns>ボーン数</returns>
		GSuint bone_count() const;

		/// <summary>
		/// ルートモーションの適用
		/// </summary>
		/// <param name="transform">= トランスフォーム</param>
		void root_motion(GStransform& transform);

		/// <summary>
		/// アニメーションイベントを追加
		/// </summary>
		/// <param name="motion">= イベントを発火する対象のモーション番号</param>
		/// <param name="time">= イベントを発火する時間</param>
		/// <param name="callback">= イベント処理</param>
		void add_event(GSuint motion, GSfloat time, function<void()> callback);

	private:
		// アニメーション
		GSuint animation_{ 0 };
		// モーション番号
		GSuint motion_{ 0 };
		// モーションタイマー
		GSfloat motion_timer_{ 0.0f };
		// 前回フレームのモーションタイマー
		GSfloat prev_timer_;
		// モーションループフラグ
		bool motion_loop_{ true };
		// 前回再生したモーション番号
		GSuint prev_motion_{ 0 };
		// 前回再生した最終アニメーションタイマー
		GSfloat prev_motion_timer_{ 0.0f };
		// 補間アニメーションタイマー
		GSfloat lerp_timer_{ 0.0f };
		// 複数のアニメーションイベントを格納するためのvector
		vector<shared_ptr<AnimationEvent>> events_;
	};

private:
	/// <summary>
	/// 複数のレイヤーが同じモーションを再生している場合再生時間を同期させる
	/// </summary>
	/// <param name="check_layer">= レイヤー番号</param>
	void sync_layers_playing_same_motion(GSuint check_layer);

	/// <summary>
	/// 複数のレイヤーが同じモーションを再生しているか
	/// </summary>
	/// <param name="layer">= レイヤー番号</param>
	/// <param name="layers">= 同じモーションを再生しているレイヤー番号</param>
	/// <returns>再生していたら真</returns>
	bool is_multiple_layers_playing_same_motion(GSuint check_layer, vector<GSuint>& layers) const;

private:
	// メッシュ
	GSuint mesh_;
	// スケルトン
	GSuint skeleton_;
	// アニメーションレイヤー
	vector<Animation> animation_layers_;
	// アニメーションレイヤーインデックス
	vector<GSuint> animation_layer_indices_;
	// ボーンのローカル変換行列
	vector<GSmatrix4> local_bone_matrices_;
	// ボーンの変換行列
	vector<GSmatrix4> bone_matrices_;
	// 座標変換行列
	GSmatrix4 transform_;

};

#endif
