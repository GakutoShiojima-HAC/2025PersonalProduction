// -----------------------------------------------------------------------------------------
//  File          : SceneState.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : シーンで使うステート定義
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCENE_STATE_H_
#define SCENE_STATE_H_

#include "Engine/Core/StateMachine/IState.h"

class SceneState : public IState {
public:
    SceneState() = default;

public:
    enum class State {
        Original,       // シーン独自の画面

        GamePause,      // ゲーム中のポーズ画面

        Setting,        // 設定画面
        Guide,          // 操作説明画面

        MAX_SIZE	    // 要素数
    };

public:
    /// <summary>
    /// GUIの描画(使わないのでvirtualを付けない)
    /// </summary>
    void draw_gui() const override {};

};

using SceneStateType = SceneState::State;

#endif
