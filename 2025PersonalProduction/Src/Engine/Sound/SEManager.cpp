#define MINIAUDIO_IMPLEMENTATION
#include "SEManager.h"

SEManager& SEManager::get_instance() {
    // static変数のインスタンスは１つ
    // インスタンス化も１回のみ
    static SEManager self;
    return self;
}

void SEManager::init() {
    ma_engine_init(NULL, &engine);
}

bool SEManager::load(unsigned int id, const std::string& path, int pool_count) {
    // 読み込み済みなら上書きしないで終了
    if (sound_map_.find(id) != sound_map_.end()) return false;

    SoundPool pool;
    pool.index = 0;

    for (int i = 0; i < pool_count; ++i) {
        ma_sound* sound = new ma_sound();

        // TODO フラグなにこれ ストリームかデコードか
        ma_result result = ma_sound_init_from_file(&engine, path.c_str(), 0, NULL, NULL, sound);
        if (result != MA_SUCCESS) {
            delete sound;
            continue; // 失敗したらスキップ
        }

        ma_sound_set_looping(sound, false); // SEはループしない
        pool.sounds.push_back(sound);
    }

    if (!pool.sounds.empty()) {
        sound_map_[id] = pool;
        return true;
    }

    return false;
}

void SEManager::play(unsigned int id, float pitch) {
    if (sound_map_.find(id) == sound_map_.end()) return;

    SoundPool& pool = sound_map_[id];
    if (pool.sounds.empty()) return;

    // カーソル位置のサウンドを取得
    ma_sound* s = pool.sounds[pool.index];

    // ピッチを指定
    ma_sound_set_pitch(s, pitch);

    // 強制的に巻き戻して再生(一番古い音なので上書き 困るならロード時の数増やそう)
    ma_sound_seek_to_pcm_frame(s, 0);
    ma_sound_start(s);

    // プールのインデックスを進める
    pool.index++;
    if (pool.index >= pool.sounds.size()) {
        pool.index = 0;
    }
}

void SEManager::play(unsigned int id, float pitch, float x, float y, float z) {
    if (sound_map_.find(id) == sound_map_.end()) return;

    SoundPool& pool = sound_map_[id];
    if (pool.sounds.empty()) return;

    ma_sound* s = pool.sounds[pool.index];

    // 座標を指定
    ma_sound_set_position(s, x, y, z);
    // ピッチを指定
    ma_sound_set_pitch(s, pitch);
    // その他設定(魔法らしい)
    ma_sound_seek_to_pcm_frame(s, 0);
    ma_sound_start(s);

    pool.index++;
    if (pool.index >= pool.sounds.size()) {
        pool.index = 0;
    }
}

void SEManager::set_listener(float x, float y, float z, float v_x, float v_y, float v_z) {
    ma_engine_listener_set_position(&engine, 0, x, y, z);
    ma_engine_listener_set_direction(&engine, 0, v_x, v_y, v_z);
}

void SEManager::delete_se(unsigned int id) {
    auto it = sound_map_.find(id);
    if (it == sound_map_.end()) return;

    SoundPool& pool = it->second;

    for (auto& s : pool.sounds) {
        ma_sound_uninit(s);
        delete s;
    }
    pool.sounds.clear();
    sound_map_.erase(it);
}

void SEManager::clear() {
    for (auto& pair : sound_map_) {
        SoundPool& pool = pair.second;
        for (ma_sound* s : pool.sounds) {
            ma_sound_uninit(s);
            delete s;
        }
        pool.sounds.clear();
    }
    sound_map_.clear();
}

void SEManager::end() {
    clear();
    ma_engine_uninit(&engine);
}
