// -----------------------------------------------------------------------------------------
//  File          : Screen.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : スクリーン情報管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SCREEN_H_
#define SCREEN_H_

#include <windows.h>

/// <summary>
/// スクリーンデータ構造体
/// </summary>
struct ScreenData {
    // ウィンドウの幅(ピクセル)
    int width_px{ 0 };
    // ウィンドウの高さ(ピクセル)
    int height_px{ 0 };
    // フルスクリーンかどうか
    bool full_screen{ false };
    // リフレッシュレート
    float refresh_rate{ 60.0f };
};

class Screen {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    Screen() = default;

public:
    ~Screen();

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static Screen& get_instance();

public:
    /// <summary>
    /// アプリケーション起動時のスクリーンデータをセットする
    /// </summary>
    /// <param name="width_px">= ウィンドウの初期幅(ピクセル)</param>
    /// <param name="height_px">= ウィンドウの初期高さ(ピクセル)</param>
    /// <param name="full_screen">= フルスクリーンにするなら真</param>
    /// <param name="refresh_rate">= アプリケーションのリフレッシュレート</param>
    void set_initialize_data(int width_px, int height_px, bool full_screen, float refresh_rate);

    /// <summary>
    /// スクリーン情報を更新
    /// </summary>
    void update();

    /// <summary>
    /// アプリケーション起動時のスクリーンデータを取得する
    /// </summary>
    /// <returns>スクリーンデータ</returns>
    const ScreenData& get_initialize_data() const;

    /// <summary>
    /// 現在のスクリーンデータを取得する
    /// </summary>
    /// <returns>スクリーンデータ</returns>
    ScreenData& get_current_data();

private:
    // 起動時のスクリーンデータ
    ScreenData initialize_data_;
    // 現在のスクリーンデータ
    ScreenData current_data_;

    // ウィンドウハンドラ
    HWND hDWnd{};
    // 現在の画面サイズ
    RECT rect_{};
    // 現在の画面座標
    RECT screen_pos_{};

public:
    // コピー禁止
    Screen(const Screen& other) = delete;
    Screen& operator = (const Screen& other) = delete;

};

#endif
