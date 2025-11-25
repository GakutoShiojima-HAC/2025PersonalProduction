// -----------------------------------------------------------------------------------------
//  File          : BGMManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/26
//  Updated       : 2025/11/26
//  Description   : ゲームのBGMを管理
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef BGMMANAGER_H_
#define BGMMANAGER_H_

class BGMManager {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    BGMManager() = default;

public:
    ~BGMManager() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static BGMManager& get_instance();

public:
    /// <summary>
    /// 再生
    /// </summary>
    /// <param name="id">= BGM番号</param>
    void play(unsigned int id);

    /// <summary>
    /// 再生 前のBGMをフェードアウトさせる
    /// </summary>
    /// <param name="id">= BGM番号</param>
    /// <param name="transition_time">= フェードアウト時間</param>
    void play(unsigned int id, float transition_time);

public:
    /// <summary>
    /// 再生中かどうか
    /// </summary>
    /// <returns>再生中なら真を返却</returns>
    bool is_play() const;

    /// <summary>
    /// 再生中のBGM番号を返却
    /// </summary>
    /// <returns>番号</returns>
    unsigned int current() const;

private:
    // 再生中のBGM
    unsigned int current_{ 0 };

    // 音量
    float volume_{ 1.0f };

public:
	// コピー禁止
	BGMManager(const BGMManager& other) = delete;
	BGMManager& operator = (const BGMManager& other) = delete;

};

#endif
