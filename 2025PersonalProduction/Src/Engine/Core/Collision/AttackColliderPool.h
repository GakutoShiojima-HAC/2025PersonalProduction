// -----------------------------------------------------------------------------------------
//  File          : AttackColliderPool.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/12
//  Updated       : 2025/08/12
//  Description   : 攻撃判定のプール
//					メモリ解放はプール内で行っているため不要
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ATTACK_COLLIDER_POOL_H_
#define ATTACK_COLLIDER_POOL_H_

#include "Engine/Utils/ObjectPool.h"
#include <list>

using namespace std;

class World;
class AttackCollider;

class AttackColliderPool {
public:
	AttackColliderPool(World* world);

	~AttackColliderPool();

public:
	void remove();

	void clear();

public:
	/// <summary>
	/// 攻撃判定を生成
	/// </summary>
	/// <param name="radius">= 判定の半径</param>
	/// <param name="center">= 生成判定球の中心</param>
	/// <param name="owner">= 生成主</param>
	/// <param name="damage">= ダメージ値</param>
	/// <param name="lifespan">= 判定の寿命</param>
	/// <param name="delay">= 判定が有効になるまでの遅延</param>
	void generate(float radius, const GSvector3& center, Actor* owner, int damage, const std::string& name, float lifespan = 1.0f, float delay = 0.0f);

private:
	World* world_{ nullptr };

	// コライダープール
	ObjectPool<AttackCollider> colliders_;
	// アクティブなコライダーリスト
	list<AttackCollider*> active_colliders_;

public:
	// コピー禁止
	AttackColliderPool(const AttackColliderPool& other) = delete;
	AttackColliderPool& operator = (const AttackColliderPool& other) = delete;

};

#endif
