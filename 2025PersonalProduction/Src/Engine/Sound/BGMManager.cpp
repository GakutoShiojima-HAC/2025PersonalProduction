#include "BGMManager.h"
#include <gslib.h>
#include "Engine/Core/Tween/Tween.h"
#include "GameConfig.h"
#include "Assets.h"

BGMManager& BGMManager::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static BGMManager self;
	return self;
}

void BGMManager::play(unsigned int id) {
    current_ = id;
    gsPlayBGM(id);
}

void BGMManager::play(unsigned int id, float transition_time) {
    current_ = id;

    // ボリュームを記憶
    volume_ = gsGetVolumeBGM();

    if (id == (GSuint)BGMID::NONE) {
        Tween::value(volume_, 0.0f, transition_time * cFPS, gsSetVolumeBGM).name("PlayBGM").on_complete(
            [=] { gsSetVolumeBGM(volume_); gsStopBGM(); }
        );
    }
    else {
        Tween::value(volume_, 0.0f, transition_time * cFPS, gsSetVolumeBGM).name("PlayBGM").on_complete(
            [=] { gsSetVolumeBGM(volume_); gsPlayBGM(id); }
        );
    }

}

bool BGMManager::is_play() const {
    return gsIsPlayBGM();
}

unsigned int BGMManager::current() const {
    return current_;
}
