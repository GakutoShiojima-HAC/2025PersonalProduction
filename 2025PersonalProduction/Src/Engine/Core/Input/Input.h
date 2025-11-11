// PC入力はPAD接続がない場合のみ適用される
//
//

#ifndef INPUT_H_
#define INPUT_H_

#include <gslib.h>

enum class InputAction {
    APP_Pause,          // ポーズ画面
    APP_ForceEnd,       // アプリケーション強制終了

    MENU_Decision,      // 決定
    MENU_ANY,           // いずれかのボタン / キー
    MENU_UP,            // 上
    MENU_DOWN,          // 下
    MENU_LEFT,          // 左
    MENU_RIGHT,         // 右

    GAME_Attack,        // 攻撃
    GAME_Jump,          // ジャンプ
    GAME_Avoid,         // 回避
    GAME_Skill,         // スキル発動
    GAME_Lockon,        // ロックオン

    GAME_Interact,      // インタラクト
    GAME_Interact_Up,   // インタラクト上選択
    GAME_Interact_Down, // インタラクト下選択
    GAME_Inventory,     // インベントリ

    DEBUG_CameraActive, // カメラ有効化
    DEBUG_Up,           // 上昇
    DEBUG_Down,         // 下降


};

class Input {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    Input() = default;

public:
    ~Input() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static Input& get_instance();

public:
    void update(float delta_time);

public:
    /// <summary>
    /// 左軸の状態を返却
    /// </summary>
    /// <returns>1.0f ~ -1.0f</returns>
    const GSvector2& left_axis();

    /// <summary>
    /// 右軸の状態を返却
    /// </summary>
    /// <returns>1.0f ~ -1.0f</returns>
    const GSvector2& right_axis();

    /// <summary>
    /// デバッグ用の左軸の状態を返却
    /// 必ずPC入力が返却される
    /// </summary>
    /// <returns>1.0f ~ -1.0f</returns>
    const GSvector2 debug_left_axis() const;

    /// <summary>
    /// デバッグ用の右軸の状態を返却
    /// 必ずPC入力が返却される
    /// </summary>
    /// <returns>1.0f ~ -1.0f</returns>
    const GSvector2 debug_right_axis() const;

    /// <summary>
    /// カーソル座標を返却
    /// </summary>
    /// <returns>座標</returns>
    const GSvector2& cursor_position();

    /// <summary>
    /// カーソル座標をリセット
    /// </summary>
    void reset_cursor_position(const GSvector2& reset_position);

    /// <summary>
    /// カーソル座標の更新フラグを取得
    /// </summary>
    /// <returns>真で更新</returns>
    bool& enable_update_cursor_position();

    /// <summary>
    /// PAD状態のカーソル移動速度を取得
    /// </summary>
    /// <returns>移動速度</returns>
    float& pad_cursor_speed();

    /// <summary>
    /// パッドが接続中かを取得
    /// </summary>
    /// <returns>接続しているなら真を返却</returns>
    bool is_pad() const;

    /// <summary>
    /// マウスカーソルを描画
    /// </summary>
    void draw_cursor() const;

public:
    /// <summary>
    /// 指定したアクションを行っているかどうか
    /// </summary>
    /// <param name="action">= アクション</param>
    /// <returns>行っているなら真を返却</returns>
    bool action(InputAction action) const;

private:
    // PAD接続中かどうか
    bool is_pad_{ false };

    // PADの左スティック・PCのWASD情報
    GSvector2 left_axis_;
    // PADの右スティック・PCのマウス情報
    GSvector2 right_axis_;
    // マウスのスクロール情報
    int scroll_{ 0 };

    // カーソル座標の更新を行うかどうか
    bool is_update_cursor_position_{ true };
    // カーソル座標
    GSvector2 cursor_position_;
    // PAD状態のカーソル移動速度
    float pad_cursor_speed_{ 1.0f };

public:
    // コピー禁止
    Input(const Input& other) = delete;
    Input& operator = (const Input& other) = delete;

};

#endif
