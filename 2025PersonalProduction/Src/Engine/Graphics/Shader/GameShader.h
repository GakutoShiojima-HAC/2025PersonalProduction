#ifndef GAME_SHADER_H_
#define GAME_SHADER_H_

typedef	unsigned int GSuint;

class GameShader {
private:
    // コンストラクタ（外部からのインスタンスを禁止）
    GameShader() = default;

public:
    ~GameShader() = default;

public:
    /// <summary>
    /// インスタンスを取得するためのstaticメンバ関数
    /// </summary>
    /// <returns>インスタンス</returns>
    static GameShader& get_instance();

public:
    /// <summary>
    /// シェーダーのロード
    /// </summary>
    void load();

    /// <summary>
    /// シェーダーのクリア
    /// </summary>
    void clear();

    /// <summary>
    /// シェーダーの使用を開始
    /// デフォルトシェーダーを上書きする
    /// </summary>
    void start();

    /// <summary>
    /// シェーダーの使用を終了
    /// デフォルトシェーダーに戻す
    /// </summary>
    void end();

private:
    // デフォルトシェーダー
    GSuint default_instacedmesh_{ 0 };
    GSuint default_mesh_{ 0 };
    GSuint default_skinmesh_{ 0 };
    GSuint default_octree_{ 0 };

public:
    // コピー禁止
    GameShader(const GameShader& other) = delete;
    GameShader& operator = (const GameShader& other) = delete;

};

#endif
