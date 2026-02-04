// -----------------------------------------------------------------------------------------
//  File          : npc.h
//  Author        : Shiojima Gakuto
//  Created       : 2026/02/03
//  Updated       : 2026/02/03
//  Description   : NPCクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef NPC_H_
#define NPC_H_

#include "Engine/Core/Actor/Pawn/Character/Character.h"

struct NPCData {
    unsigned int skinmesh{ 0 };         // スキンメッシュID

    unsigned int motion_idle{ 0 };      // 待機中のモーション
    unsigned int motion_action{ 1 };    // インタラクトに反応するモーション

    std::string name{ "MyEnemy" };      // 名前
    std::string pawn_type{ "Normal" };  // ポーンタイプ

    std::string timeline{ "A" };        // インタラクト時に再生するタイムライン
};

class NPC : public Character {
public:
    NPC(IWorld* world, const GSvector3& position, const GSvector3& rotate, const NPCData& data);

    ~NPC() = default;

public:
    void update(float delta_time) override;

    void draw() const override;

public:
    void message(const std::string& message, std::any& param) override;

    bool can_interact() const override;

    std::string interact_text() const override;

public:
    /// <summary>
    /// アクションが終了したかどうか
    ///
    bool is_action_end() const;

    /// <summary>
    /// アクションを終了
    /// </summary>
    void end_action();

    const NPCData& data() const;

private:
    void add_state() override;

private:
    bool is_interact_{ false };

    NPCData data_;

    // ターゲット
    Character* target_{ nullptr };

    // 最初の回転
    GSvector3 origin_rotate_{ 0.0f, 0.0f, 0.0f };
};


#endif
