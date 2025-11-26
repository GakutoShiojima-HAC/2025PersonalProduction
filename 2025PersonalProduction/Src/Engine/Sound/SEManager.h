// -----------------------------------------------------------------------------------------
//  File          : SEManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/27
//  Updated       : 2025/11/27
//  Description   : ゲームのSEを管理
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef SE_MANAGER_H_
#define SE_MANAGER_H_

#include "Lib/miniaudio.h"
#include <vector>
#include <unordered_map>
#include <string>

class SEManager {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
   SEManager() = default;

public:
    ~SEManager() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static SEManager& get_instance();

public:
    /// <summary>
    /// 初期化
    /// </summary>
    void init();

    /// <summary>
    /// 読み込み
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="path">= ファイルパス</param>
    /// <param name="pool_count">= 同時再生可能回数</param>
    /// <returns>読み込みに成功したら真を返却</returns>
    bool load(unsigned int id, const std::string& path, int pool_count = 1);

    /// <summary>
    /// 再生(2D)
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="pitch">= ピッチ</param>
    void play(unsigned int id, float pitch);

    /// <summary>
    /// 再生(3D)
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="pitch">= ピッチ</param>
    /// <param name="x">= 3D空間上のX座標</param>
    /// <param name="y">= 3D空間上のY座標</param>
    /// <param name="z">= 3D空間上のZ座標</param>
    void play(unsigned int id, float pitch, float x, float y, float z);

    /// <summary>
    /// リスナーの位置を指定
    /// </summary>
    /// <param name="x">= 3D空間上のX座標</param>
    /// <param name="y">= 3D空間上のY座標</param>
    /// <param name="z">= 3D空間上のZ座標</param>
    /// <param name="v_x">= リスナーの向いている方向ベクトルのX成分</param>
    /// <param name="v_y">= リスナーの向いている方向ベクトルのY成分</param>
    /// <param name="v_z">= リスナーの向いている方向ベクトルのZ成分</param>
    void set_listener(float x, float y, float z, float v_x, float v_y, float v_z);

    /// <summary>
    /// 破棄
    /// </summary>
    /// <param name="id">= ID</param>
    void delete_se(unsigned int id);

    /// <summary>
    /// 全て破棄
    /// </summary>
    void clear();

    /// <summary>
    /// 終了
    /// </summary>
    void end();

private:
    struct SoundPool {
        std::vector<ma_sound*> sounds;
        int index = 0;  // 次にどれを鳴らすか
    };

private:
    ma_engine engine;

    std::unordered_map<unsigned int, SoundPool> sound_map_;

public:
    // コピー禁止
    SEManager(const SEManager& other) = delete;
    SEManager& operator = (const SEManager& other) = delete;

};


#endif
