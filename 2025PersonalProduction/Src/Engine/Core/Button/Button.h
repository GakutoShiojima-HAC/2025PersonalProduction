// -----------------------------------------------------------------------------------------
//  File          : Button.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/10/20
//  Updated       : 2025/10/20
//  Description   : ボタンの親クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef BUTTON_H_
#define BUTTON_H_

typedef struct GSvector2;
typedef struct GSrect;

class Button {
public:
    Button() = default;

    virtual ~Button() = default;

public:
    /// <summary>
    /// 初期状態の選択初期状態の選択(音などを鳴らさないため)
    /// </summary>
    void init_select();

    /// <summary>
    /// ゲームパッドで呼ぶ更新
    /// </summary>
    /// <param name="is_selected">= 選択対象のボタンかどうか</param>
    void update(float delta_time, bool is_selected);

    /// <summary>
    /// マウスで呼ぶ更新
    /// </summary>
    /// <param name="cursor_position">= マウスカーソルの座標</param>
    void update(float delta_time, const GSvector2& cursor_position);

    /// <summary>
    /// 入力があった時
    /// </summary>
    void on_input_decision();

    /// <summary>
    /// 描画
    /// </summary>
    virtual void draw() const {};

protected:
    /// <summary>
    /// ボタンが選択されたとき
    /// </summary>
    virtual void select() {};

    /// <summary>
    /// ボタンが選択されているとき
    /// </summary>
    virtual void state(float delta_time) {};

    /// <summary>
    /// ボタンが入力されたとき
    /// </summary>
    virtual void input() {};

    /// <summary>
    /// ボタンの選択が外れたとき
    /// </summary>
    virtual void exit() {};

    /// <summary>
    /// このボタンが選択中かどうか
    /// </summary>
    /// <returns>選択中なら真を返却</returns>
    bool is_selected() const;

protected:
    // ボタンの範囲
    GSrect rect_{ 0.0f, 0.0f, 0.0f, 0.0f };

private:
    // このボタンを選択中かどうか
    bool is_selected_{ false };

public:
    // コピー禁止
    Button(const Button& other) = delete;
    Button& operator = (const Button& other) = delete;

};

#endif
