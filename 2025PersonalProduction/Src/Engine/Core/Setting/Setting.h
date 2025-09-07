#ifndef SETTING_H_
#define SETTING_H_

#include <string>

class Setting {
private:
	// �R���X�g���N�^�i�O������̃C���X�^���X���֎~�j
	Setting() = default;

public:
	~Setting() = default;

public:
	/// <summary>
	/// �C���X�^���X���擾���邽�߂�static�����o�֐�
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Setting& get_instance();

public:
	/// <summary>
	/// �ݒ��ǂݍ���
	/// </summary>
	/// <param name="setting_path">= �ݒ�t�@�C���̃p�X</param>
	void load(const std::string& setting_path);

	/// <summary>
	/// �ݒ��������Ԃɖ߂�
	/// </summary>
	void reset();

	/// <summary>
	/// �ݒ��ۑ�����
	/// </summary>
	void save();

	// �V�[���`��֌W
public: 
	bool is_draw_posteffect() const;
	bool& enable_draw_posteffect();
	
	bool is_draw_fxaa() const;
	bool& enable_draw_fxaa();

private:
	bool enable_draw_posteffect_{ true };
	bool enable_draw_fxaa_{ true };

public:
	// �R�s�[�֎~
	Setting(const Setting& other) = delete;
	Setting& operator = (const Setting& other) = delete;

};

#endif

