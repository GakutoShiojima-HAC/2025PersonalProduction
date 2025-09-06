#ifndef WEAPON_MANAGER_H_
#define WEAPON_MANAGER_H_

#include "Weapon/WeaponType.h"
#include <gslib.h>
#include <vector>
#include <unordered_map>

class WeaponManager {
public:
	WeaponManager() = default;

	~WeaponManager();

public:
	struct WeaponAnimationData {
		// �A�j���[�V�����ԍ�
		int animation_num{ -1 };
		// �U������𐶐�����L�[�t���[��
		float attack_frame{ -1.0f };
		// �U������𐶐����鑊�΍��W
		GSvector3 collider_offset{ 0.0f, 0.0f, 0.0f };
		// ���̕���A�j���[�V�����ɑJ�ڂł���܂ł̎���(delta_time)
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
	/// ����̎�ނƃp�����[�^��ǉ�
	/// </summary>
	/// <param name="type">= �ǉ����镐��̎��</param>
	/// <param name="data">= �����̃p�����[�^</param>
	void add_weapon_parameter(WeaponType type, const std::vector<WeaponAnimationData*>& data);

	/// <summary>
	/// ����̎�ނ���ő�U���\�i�����擾
	/// </summary>
	/// <param name="type">= ����̎��</param>
	/// <returns>�ő�U���\�i��</returns>
	int get_max_attack_count(WeaponType type);

	/// <summary>
	/// ����̎�ނƍU���i������Đ�����A�j���[�V�����ԍ����擾
	/// </summary>
	/// <param name="type">����̎��</param>
	/// <param name="attack_count">= �U���i��</param>
	/// <returns>�Đ�����A�j���[�V�����ԍ� ���݂��Ȃ���Ε��̒l��ԋp</returns>
	int get_animation_num(WeaponType type, int attack_count);

	/// <summary>
	/// ����̎�ނƍU���i������U������𔭐�������t���[�����擾
	/// </summary>
	/// <param name="type">= ����̎��</param>
	/// <param name="attack_count">= �U���i��</param>
	/// <returns>����𔭐�������t���[�� ���݂��Ȃ���Ε��̒l��ԋp</returns>
	float get_attack_frame(WeaponType type, int attack_count);

	/// <summary>
	/// ����̎�ނƍU���i������U������𐶐����鑊�΍��W���擾
	/// </summary>
	/// <param name="type">= ����̎��</param>
	/// <param name="attack_count">= �U���i��</param>
	/// <returns>�����ʒu�̑��΍��W</returns>
	GSvector3 get_collider_offset(WeaponType type, int attack_count);

	/// <summary>
	/// ����̎�ނƍU���i�����玟�̍U�����[�V�����ɓ���܂ł̎��Ԃ��擾
	/// </summary>
	/// <param name="type">= ����̎��</param>
	/// <param name="attack_count">= �U���i��</param>
	/// <returns>���̍U�����[�V�����ɓ���܂ł̎���</returns>
	float get_enter_next_animation_time(WeaponType type, int attack_count);

	/// <summary>
	///  �o�^����Ă��镐��̎�ނ�ԋp
	/// </summary>
	/// <returns>����̎�ޕ��̐��l</returns>
	int count();

private:
	/// <summary>
	/// ���킪�o�^����Ă��邩
	/// </summary>
	/// <param name="type">= ����̎��</param>
	/// <returns>�o�^����Ă�����^��ԋp</returns>
	bool has_weapon(WeaponType type) const;

private:
	std::unordered_map<WeaponType, std::vector<WeaponAnimationData*>> data_;

public:
	// �R�s�[�֎~
	WeaponManager(const WeaponManager& other) = delete;
	WeaponManager& operator = (const WeaponManager& other) = delete;
};

#endif
