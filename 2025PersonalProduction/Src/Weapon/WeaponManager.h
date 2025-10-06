// -----------------------------------------------------------------------------------------
//  File          : WeaponManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/06
//  Updated       : 2025/09/07
//  Description   : 武器の種類とアニメーションや生成位置を管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef WEAPON_MANAGER_H_
#define WEAPON_MANAGER_H_

#include "Item/Weapon/WeaponData.h"
#include <gslib.h>
#include <vector>
#include <unordered_map>

class WeaponManager {
public:
	WeaponManager() = default;

	~WeaponManager();

public:
	struct WeaponAnimationData {
		// アニメーション番号
		int animation_num{ -1 };
		// 攻撃判定を生成するキーフレーム
		float attack_frame{ -1.0f };
		// 攻撃判定を生成する相対座標
		GSvector3 collider_offset{ 0.0f, 0.0f, 0.0f };
		// 次の武器アニメーションに遷移できるまでの時間(delta_time)
		float enter_next_animation_time{ 0.0f };

		WeaponAnimationData() {}

		WeaponAnimationData(int animation_num, float attack_frame, const GSvector3& collider_offset, float enter_next_animation_time) :
			animation_num{ animation_num },
			attack_frame{ attack_frame },
			collider_offset{ collider_offset },
			enter_next_animation_time{ enter_next_animation_time } {
		}
	};

public:
	void clear();

public:
	/// <summary>
	/// 武器の種類とパラメータを追加
	/// </summary>
	/// <param name="type">= 追加する武器の種類</param>
	/// <param name="data">= 武器種のパラメータ</param>
	void add_weapon_parameter(WeaponType type, const std::vector<WeaponAnimationData*>& data);

	/// <summary>
	/// 武器の種類から最大攻撃可能段数を取得
	/// </summary>
	/// <param name="type">= 武器の種類</param>
	/// <returns>最大攻撃可能段数</returns>
	int get_max_attack_count(WeaponType type);

	/// <summary>
	/// 武器の種類と攻撃段数から再生するアニメーション番号を取得
	/// </summary>
	/// <param name="type">武器の種類</param>
	/// <param name="attack_count">= 攻撃段数</param>
	/// <returns>再生するアニメーション番号 存在しなければ負の値を返却</returns>
	int get_animation_num(WeaponType type, int attack_count);

	/// <summary>
	/// 武器の種類と攻撃段数から攻撃判定を発生させるフレームを取得
	/// </summary>
	/// <param name="type">= 武器の種類</param>
	/// <param name="attack_count">= 攻撃段数</param>
	/// <returns>判定を発生させるフレーム 存在しなければ負の値を返却</returns>
	float get_attack_frame(WeaponType type, int attack_count);

	/// <summary>
	/// 武器の種類と攻撃段数から攻撃判定を生成する相対座標を取得
	/// </summary>
	/// <param name="type">= 武器の種類</param>
	/// <param name="attack_count">= 攻撃段数</param>
	/// <returns>生成位置の相対座標</returns>
	GSvector3 get_collider_offset(WeaponType type, int attack_count);

	/// <summary>
	/// 武器の種類と攻撃段数から次の攻撃モーションに入るまでの時間を取得
	/// </summary>
	/// <param name="type">= 武器の種類</param>
	/// <param name="attack_count">= 攻撃段数</param>
	/// <returns>次の攻撃モーションに入るまでの時間</returns>
	float get_enter_next_animation_time(WeaponType type, int attack_count);

	/// <summary>
	///  登録されている武器の種類を返却
	/// </summary>
	/// <returns>武器の種類分の数値</returns>
	int count();

private:
	/// <summary>
	/// 武器が登録されているか
	/// </summary>
	/// <param name="type">= 武器の種類</param>
	/// <returns>登録されていたら真を返却</returns>
	bool has_weapon(WeaponType type) const;

private:
	std::unordered_map<WeaponType, std::vector<WeaponAnimationData*>> data_;

public:
	// コピー禁止
	WeaponManager(const WeaponManager& other) = delete;
	WeaponManager& operator = (const WeaponManager& other) = delete;
};

#endif
