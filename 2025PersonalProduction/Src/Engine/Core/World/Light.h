// -----------------------------------------------------------------------------------------
//  File        : Light.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/29
//  Updated     : 2025/06/29
//  Description : ライト
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LIGHT_H_
#define LIGHT_H_

#include <GSvector4.h>
#include <GSvector3.h>

class Light {
public:
	Light(
		GSvector4 ambient = GSvector4{ 0.2f, 0.2f, 0.2f, 1.0f },
		GSvector4 diffuse = GSvector4{ 1.0f, 1.0f, 1.0f, 1.0f },
		GSvector4 specular = GSvector4{ 1.0f, 1.0f, 1.0f, 1.0f },
		GSvector3 position = GSvector3{ 200.0f, 200.0f, 200.0f }
	);

	virtual ~Light() = default;

public:
	/// <summary>
	/// 描画
	/// </summary>
	virtual void draw() const;

public:
	/// <summary>
	/// ライトの色を取得
	/// </summary>
	GSvector4& ambient();

	/// <summary>
	/// ライトの拡散反射色を取得
	/// </summary>
	GSvector4& diffuse();

	/// <summary>
	/// ライトの鏡面反射色を取得
	/// </summary>
	GSvector4& specular();

	/// <summary>
	/// ライトの座標を取得
	/// </summary>
	GSvector3& position();

protected:
	// 色
	GSvector4 ambient_{ 0.2f, 0.2f, 0.2f, 1.0f };
	// 拡散反射色
	GSvector4 diffuse_{ 1.0f, 1.0f, 1.0f, 1.0f };
	// 鏡面反射色
	GSvector4 specular_{ 1.0f, 1.0f, 1.0f, 1.0f };
	// 座標
	GSvector3 position_{ -129.0f, 231.0f, -141.0f };

public:
	// コピー禁止
	Light(const Light& other) = delete;
	Light& operator = (const Light& other) = delete;

};

#endif
