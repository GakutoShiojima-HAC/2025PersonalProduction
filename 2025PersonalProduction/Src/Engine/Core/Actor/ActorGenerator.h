#ifndef ACTOR_GENERATOR_H_
#define ACTOR_GENERATOR_H_

#include <unordered_map>
#include <string>
#include "Engine/Core/Actor/IActorGenerator.h"

class ActorGenerator {
public:
    ActorGenerator() = default;

    ~ActorGenerator();

public:
    /// <summary>
    /// 生成に必要なデータを読み込む
    /// </summary>
    /// <param name="world">= 生成先のワールド</param>
    void load(World* world);

    /// <summary>
    /// 生成する
    /// </summary>
    /// <param name="json_file">= 生成方法が書かれているjsonファイル</param>
    void generate(const std::string& json_file);

    /// <summary>
    /// 生成する
    /// </summary>
    /// <param name="actor_key">= 識別キー</param>
    /// <param name="position">= 座標</param>
    /// <param name="lookat">= 注視点</param>
    /// <param name="hp">= 体力</param>
    /// <param name="damage">= 攻撃力</param>
    void generate(const std::string& actor_key, const GSvector3& position, const GSvector3& lookat, int hp = 1, int damage = 1);

    /// <summary>
    /// 生成で使ったデータの破棄
    /// </summary>
    void clear();

private:
    // 生成用データ
    std::unordered_map<std::string, IActorGenerator*> data_;

public:
    // コピー禁止
    ActorGenerator(const ActorGenerator& other) = delete;
    ActorGenerator& operator = (const ActorGenerator& other) = delete;

};

#endif
