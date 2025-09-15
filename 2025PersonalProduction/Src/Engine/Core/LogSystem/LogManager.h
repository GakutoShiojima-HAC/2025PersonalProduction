// -----------------------------------------------------------------------------------------
//  File        : LogManager.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/09/15
//  Updated     : 2025/09/15
//  Description : アプリケーション全体のログの管理、出力を行う
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <string>

class LogManager {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    LogManager() = default;

public:
    ~LogManager() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static LogManager& get_instance();

public:
    /// <summary>
    /// アプリケーションの開始
    /// </summary>
    void app_start();

    /// <summary>
    /// ログの保存
    /// </summary>
    /// <param name="folder_path">= 出力先のフォルダ</param>
    void save(const std::string& folder_path);

public:
    /// <summary>
    /// ランダムのシード値を設定
    /// </summary>
    /// <param name="seed">= シード値</param>
    void set_seed(unsigned int seed);


private:
    // アプリケーション開始時刻
    std::string start_time{ "" };

    // ランダムシード値
    unsigned int random_seed_{ 0 };

public:
    // コピー禁止
    LogManager(const LogManager& other) = delete;
    LogManager& operator = (const LogManager& other) = delete;
};

#endif
