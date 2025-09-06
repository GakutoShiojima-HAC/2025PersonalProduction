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

	// ã‘‚«—p
	for (auto ptr : vec) delete ptr;
	vec.clear();

	vec.insert(vec.end(), data.begin(), data.end());
}

int WeaponManager::get_max_attack_count(WeaponType type) {
	if (!has_weapon(type)) return 0;
	return data_[type].size();
}

int WeaponManager::get_animation_num(WeaponType type, int attack_count) {
	if (!has_weapon(type)) return -1;
	return data_[type][attack_count - 1]->animation_num;
}

float WeaponManager::get_attack_frame(WeaponType type, int attack_count) {
	if (!has_weapon(type)) return -1.0f;
	return data_[type][attack_count - 1]->attack_frame;
}

GSvector3 WeaponManager::get_collider_offset(WeaponType type, int attack_count) {
	if (!has_weapon(type)) return GSvector3{ 0.0f, 0.0f, 0.0f };
	return data_[type][attack_count - 1]->collider_offset;
}

float WeaponManager::get_enter_next_animation_time(WeaponType type, int attack_count) {
	if (!has_weapon(type)) return 99999.0f;
	return data_[type][attack_count - 1]->enter_next_animation_time;
}

int WeaponManager::count() {
	return data_.size();
}

bool WeaponManager::has_weapon(WeaponType type) const {
	auto it = data_.find(type);
	return it == data_.end() ? false : true;
}
