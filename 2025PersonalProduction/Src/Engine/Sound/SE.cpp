#include "SE.h"
#include "SEManager.h"
#include "Engine/Utils/MyRandom.h"

void SE::init() {
    SEManager::get_instance().init();
}

void SE::clear() {
    SEManager::get_instance().clear();
}

void SE::end() {
    SEManager::get_instance().end();
}

void SE::set_master_volume(float volume) {
    SEManager::get_instance().set_master_volume(volume);
}

float SE::get_master_volume() {
    return SEManager::get_instance().get_master_volume();
}

bool SE::load(unsigned int id, const std::string& path, int pool_count) {
    return SEManager::get_instance().load(id, path, pool_count);
}

void SE::play(unsigned int id, float pitch) {
    SEManager::get_instance().play(id, pitch);
}

void SE::play_random(unsigned int id, float pitch_width) {
    SEManager::get_instance().play(id, MyRandom::random_float(1.0f - pitch_width, 1.0f + pitch_width));
}

void SE::play(unsigned int id, const GSvector3& position, float pitch) {
    SEManager::get_instance().play(id, pitch, position.x, position.y, position.z);
}

void SE::play_random(unsigned int id, const GSvector3& position, float pitch_width) {
    SEManager::get_instance().play(id, MyRandom::random_float(1.0f - pitch_width, 1.0f + pitch_width), position.x, position.y, position.z);
}

void SE::listener(const GStransform& transform) {
    SEManager::get_instance().set_listener(
        transform.position().x, transform.position().y, transform.position().z,
        transform.forward().x, transform.forward().y, transform.forward().z
    );
}

void SE::delete_se(unsigned int id) {
    SEManager::get_instance().delete_se(id);
}
