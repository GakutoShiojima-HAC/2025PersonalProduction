// -----------------------------------------------------------------------------------------
//  File        : Field.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/07/10
//  Updated     : 2025/07/10
//  Description : ワールドのフィールド管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>
#include "Engine/Core/Actor/FieldActor/FieldActorManager.h"

class Ray;
class Line;
class BoundingSphere;

class Field {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="octree">= オクツリー番号</param>
	/// <param name="collider">= 衝突判定用オクツリー番号</param>
	/// <param name="skybox">= スカイボックス番号</param>
	Field(GSuint octree, GSuint collider, GSuint skybox);

	~Field();

public:
	void update(float delta_time);

	void draw() const;

	void clear();

public:
	/// <summary>
	/// フィールドアクターを描画
	/// </summary>
	void draw_field_actor() const;

	/// <summary>
	/// フィールドアクターの追加
	/// </summary>
	/// <param name="actor">フィールドアクター</param>
	void add(FieldActor* field_actor);
	
	/// <summary>
	/// レイとの衝突判定
	/// </summary>
	/// <param name="ray">= レイ</param>
	/// <param name="max_distance">= レイの長さ</param>
	/// <param name="intersect">= 衝突した交点の受取先</param>
	/// <param name="plane">= 衝突したプレーンの受取先</param>
	/// <param name="actor">= 衝突したフィールドアクターの受取先</param>
	/// <returns>衝突していたら真</returns>
	bool collide(const Ray& ray, float max_distance, GSvector3* intersect = nullptr, GSplane* plane = nullptr, Actor** actor = nullptr) const;

	/// <summary>
	/// 線分との衝突判定
	/// </summary>
	/// <param name="line">= 線分</param>
	/// <param name="intersect">= 衝突した交点の受取先</param>
	/// <param name="plane">= 衝突したプレーンの受取先</param>
	/// <param name="actor">= 衝突したフィールドアクターの受取先</param>
	/// <returns>衝突していたら真</returns>
	bool collide(const Line& line, GSvector3* intersect = nullptr, GSplane* plane = nullptr, Actor** actor = nullptr) const;

	/// <summary>
	/// 球体との衝突判定
	/// </summary>
	/// <param name="sphere">= 球体</param>
	/// <param name="center">= 衝突した中心座標の受取先</param>
	/// <param name="actor">= 衝突したフィールドアクターの受取先</param>
	/// <returns>衝突していたら真</returns>
	bool collide(const BoundingSphere& sphere, GSvector3* center = nullptr, Actor** actor = nullptr) const;

public:
	/// <summary>
	/// 描画用オクツリーを取得
	/// </summary>
	GSuint& octree();

	/// <summary>
	/// 衝突判定用オクツリーを取得
	/// </summary>
	GSuint& collider();

	/// <summary>
	/// スカイボックスを取得
	/// </summary>
	GSuint& skybox();

private:
	// オクツリー
	GSuint octree_{ 0 };
	// 衝突判定オクツリー
	GSuint collider_{ 0 };
	// スカイボックス
	GSuint skybox_{ 0 };

	// フィールドアクター
	FieldActorManager actors_;

#ifdef _DEBUG
	// オクツリーを描画するかどうか
	bool draw_octree_{ true };
	// スカイボックスを描画するかどうか
	bool draw_skybox_{ true };
#endif

public:
	// コピー禁止
	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

};

#endif
