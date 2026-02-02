// -----------------------------------------------------------------------------------------
//  File          : Setting.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/XX/XX
//  Updated       : 2025/10/20
//  Description   : ゲーム内設定を管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SETTING_H_
#define SETTING_H_

#include <string>
#include "Engine/Core/Setting/SettingWindow.h"

class Setting {
private:
	// コンストラクタ（外部からのインスタンスを禁止）
	Setting();

public:
	~Setting();

public:
	/// <summary>
	/// インスタンスを取得するためのstaticメンバ関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Setting& get_instance();

public:
    /// <summary>
    /// 設定画面の開始
    /// </summary>
    void start();

    /// <summary>
    /// 設定画面の更新
    /// </summary>
    /// <param name="delta_time"></param>
    void update(float delta_time);

    /// <summary>
    /// 設定画面の描画
    /// </summary>
    void draw() const;

    /// <summary>
    /// 設定画面の描画を終了するかどうか
    /// </summary>
    /// <returns>終了するなら真を返却</returns>
    bool is_end() const;

public:
	/// <summary>
	/// 設定を読み込む
	/// </summary>
	/// <param name="setting_path">= 設定ファイルのパス</param>
	void load(const std::string& setting_path);

	/// <summary>
	/// 設定を初期状態に戻す
	/// </summary>
	void reset();

	/// <summary>
	/// 設定を保存する
	/// </summary>
	void save();

	// シーン描画関係
public:
    bool is_draw_ssao() const;
    bool& enable_draw_ssao();

	bool is_draw_bloom() const;
	bool& enable_draw_bloom();
	
	bool is_draw_fxaa() const;
	bool& enable_draw_fxaa();

    // デバッグ関係
public:
    bool is_draw_safearea() const;
    bool& enable_draw_safearea();

    // デバイス関連
public:
    bool is_vibration() const;
    bool& enable_vibration();

    float sens_ratio() const;
    float& sens_ratio();

private:
    SettingWindow* window_{ nullptr };
    
private:
    bool enable_draw_ssao_{ true };
	bool enable_draw_bloom_{ true };
	bool enable_draw_fxaa_{ false };

    bool enable_draw_safearea_{ false };

    bool enable_vibration_{ true };

    float sens_{ 1.0f };

public:
	// コピー禁止
	Setting(const Setting& other) = delete;
	Setting& operator = (const Setting& other) = delete;

};

#endif

