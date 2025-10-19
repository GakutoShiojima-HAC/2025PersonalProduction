#ifndef SETTING_H_
#define SETTING_H_

#include <string>

class Setting {
private:
	// コンストラクタ（外部からのインスタンスを禁止）
	Setting() = default;

public:
	~Setting() = default;

public:
	/// <summary>
	/// インスタンスを取得するためのstaticメンバ関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static Setting& get_instance();

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
    
private:
    bool enable_draw_ssao_{ true };
	bool enable_draw_bloom_{ true };
	bool enable_draw_fxaa_{ true };

    bool enable_draw_safearea_{ false };

    bool enable_vibration_{ false };

public:
	// コピー禁止
	Setting(const Setting& other) = delete;
	Setting& operator = (const Setting& other) = delete;

};

#endif

