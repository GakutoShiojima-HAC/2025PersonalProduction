// -----------------------------------------------------------------------------------------
//  File        : Light.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/06/29
//  Updated     : 2025/10/30
//  Description : ���C�g
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef LIGHT_H_
#define LIGHT_H_

#include <GSvector4.h>
#include <GSvector3.h>

class Light {
public:
	Light(
		const GSvector3& angle = GSvector3{ 30.0f, 50.0f, 0.0f },
		const GSvector4& ambient = GSvector4{ 0.0f, 0.0f, 0.0f, 1.0f },
		const GSvector4& diffuse = GSvector4{ 1.0f, 1.0f, 1.0f, 1.0f },
		const GSvector4& specular = GSvector4{ 1.0f, 1.0f, 1.0f, 1.0f }
	);

	virtual ~Light() = default;

public:
	/// <summary>
	/// �`��
	/// </summary>
	virtual void draw() const;

public:
	/// <summary>
	/// ���C�g�̐F���擾
	/// </summary>
	GSvector4& ambient();

	/// <summary>
	/// ���C�g�̊g�U���ːF���擾
	/// </summary>
	GSvector4& diffuse();

	/// <summary>
	/// ���C�g�̋��ʔ��ːF���擾
	/// </summary>
	GSvector4& specular();

	/// <summary>
	/// ���C�g�̍��W���擾
	/// </summary>
	GSvector3& position();

protected:
	// �F
	GSvector4 ambient_{ 0.0f, 0.0f, 0.0f, 1.0f };
	// �g�U���ːF
	GSvector4 diffuse_{ 1.0f, 1.0f, 1.0f, 1.0f };
	// ���ʔ��ːF
	GSvector4 specular_{ 1.0f, 1.0f, 1.0f, 1.0f };
	// ���W
	GSvector3 position_{ 0.0f, 0.0f, 0.0f };

public:
	// �R�s�[�֎~
	Light(const Light& other) = delete;
	Light& operator = (const Light& other) = delete;

};

#endif
