#include "Weapon/WeaponManager.h"

WeaponManager::~WeaponManager() {
	clear();
}

void WeaponManager::clear() {
	for (auto& data : data_) {
		for (auto ptr : data.second) {
			delete ptr;
		}
	}
	data_.clear();
}

void WeaponManager::add_weapon_parameter(WeaponType type, const std::vector<WeaponAnimationData*>& data) {
	auto& vec = data_[type];

	// 上書き用
	for (auto ptr : vec) delete ptr;
	vec.clear();

	vec.insert(vec.end(), data.begin(), data.end());
}

int WeaponManager::get_max_attack_count(WeaponType type) const {
    // 武器種が登録されていなかったらエラー値を返却
    auto it = data_.find(type);
    if (it == data_.end())  return 0;

    return it->second.size();
}

int WeaponManager::get_animation_num(WeaponType type, int attack_count) const {
    // 武器種が登録されていなかったらエラー値を返却
    auto it = data_.find(type);
    if (it == data_.end()) return -1;

    // 攻撃数が登録数の範囲外だったらエラー値を返却
    const auto& anim_vector = it->second;
    if (attack_count <= 0 || static_cast<size_t>(attack_count) > anim_vector.size()) return -1;

    return anim_vector[attack_count - 1]->animation_num;
}

float WeaponManager::get_attack_frame(WeaponType type, int attack_count) const {
    // 武器種が登録されていなかったらエラー値を返却
    auto it = data_.find(type);
    if (it == data_.end()) return -1.0f;

    // 攻撃数が登録数の範囲外だったらエラー値を返却
    const auto& anim = it->second;
    if (attack_count <= 0 || (size_t)attack_count > anim.size()) return -1.0f;

    return anim[attack_count - 1]->attack_frame;
}

GSvector3 WeaponManager::get_collider_offset(WeaponType type, int attack_count) const {
    // 武器種が登録されていなかったらエラー値を返却
    auto it = data_.find(type);
    if (it == data_.end()) return GSvector3{ 0.0f, 0.0f, 0.0f };

    // 攻撃数が登録数の範囲外だったらエラー値を返却
    const auto& anim = it->second;
    if (attack_count <= 0 || (size_t)attack_count > anim.size()) return GSvector3{ 0.0f, 0.0f, 0.0f };

    return anim[attack_count - 1]->collider_offset;
}

float WeaponManager::get_enter_next_animation_time(WeaponType type, int attack_count) const {
    // 武器種が登録されていなかったらエラー値を返却
    auto it = data_.find(type);
    if (it == data_.end()) return 99999.0f;

    // 攻撃数が登録数の範囲外だったらエラー値を返却
    const auto& anim = it->second;
    if (attack_count <= 0 || (size_t)attack_count > anim.size()) return 99999.0f;

    return anim[attack_count - 1]->enter_next_animation_time;
}

int WeaponManager::count() const {
	return data_.size();
}
