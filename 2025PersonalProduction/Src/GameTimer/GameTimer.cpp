#include "GameTimer/GameTimer.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "GameConfig.h"
#include "Engine/Graphics/Canvas/Canvas.h"

void GameTimer::init() {
    elapsed_time_ = 0.0f;
}

void GameTimer::update(float delta_time) {
    elapsed_time_ += delta_time / cFPS;
}

void GameTimer::draw() const {
    // 秒を整数にする
    int total_seconds = (int)std::floor(elapsed_time_);

    // 分と秒に分ける
    int minutes = total_seconds / 60;
    int seconds = total_seconds % 60;

    // 形式をmm::ssにする
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes
        << ":"
        << std::setfill('0') << std::setw(2) << seconds;

    Canvas::draw_sprite_text(
        ss.str(),
        GSvector2{ 100.0f, 350.0f },    // TODO
        40,
        cFONT,
        GS_FONT_BOLD
    );
}

float GameTimer::get_elapsed_time() const {
    return elapsed_time_;
}
