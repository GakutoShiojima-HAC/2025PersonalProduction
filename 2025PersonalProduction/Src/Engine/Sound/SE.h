#ifndef MY_SE_H_
#define MY_SE_H_

#include <GStransform.h>
#include <string>

class SE {
public:
    // インスタンス生成禁止
    SE() = delete;

public:
    /// <summary>
    /// 初期化
    /// </summary>
    static void init();

    /// <summary>
    /// 全て破棄
    /// </summary>
    static void clear();

    /// <summary>
    /// 終了
    /// </summary>
    static void end();

    /// <summary>
    /// マスターボリュームを設定(0.0~1.0)
    /// </summary>
    /// <param name="volume">= ボリューム</param>
    static void set_master_volume(float volume);

    /// <summary>
    /// マスターボリュームを取得
    /// </summary>
    /// <returns>0.0~1.0</returns>
    static float get_master_volume();

public:

    /// <summary>
    /// 読み込み
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="path">= ファイルパス</param>
    /// <param name="pool_count">= 同時再生可能回数</param>
    /// <returns>読み込みに成功したら真を返却</returns>
    static bool load(unsigned int id, const std::string& path, int pool_count = 1);

    /// <summary>
    /// 再生(2D)
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="pitch">= ピッチ</param>
    static void play(unsigned int id, float pitch = 1.0f);

    /// <summary>
    /// 再生(2D) 1.0からのピッチ幅でランダム再生
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="pitch_width">= ピッチ幅</param>
    static void play_random(unsigned int id, float pitch_width);

    /// <summary>
    /// 再生(3D)
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="position">= 3D空間の座標</param>
    /// <param name="pitch">= ピッチ</param>
    static void play(unsigned int id, const GSvector3& position, float pitch = 1.0f);

    /// <summary>
    /// 再生(3D) 1.0からのピッチ幅でランダム再生
    /// </summary>
    /// <param name="id">= ID</param>
    /// <param name="position">= 3D空間の座標</param>
    /// <param name="pitch_width">= ピッチ</param>
    static void play_random(unsigned int id, const GSvector3& position, float pitch_width);

    /// <summary>
    /// リスナーの設定
    /// </summary>
    /// <param name="transform">= トランスフォーム</param>
    static void listener(const GStransform& transform);

    /// <summary>
    /// 破棄
    /// </summary>
    /// <param name="id">= ID</param>
    static void delete_se(unsigned int id);

};

#endif
