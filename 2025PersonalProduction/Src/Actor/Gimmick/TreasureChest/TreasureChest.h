// -----------------------------------------------------------------------------------------
//  File          : TreasureChest.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/12/12
//  Updated       : 2025/12/12
//  Description   : 宝箱
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef TREASURECHEST_H_
#define TREASURECHEST_H_

#include "Engine/Core/Actor/Actor.h"
#include "Item/ItemData.h"

class TreasureChest : public Actor {
public:
	TreasureChest(IWorld* world, const GSvector3& position, const GSvector3& rotate, ItemData::Data& data);

	~TreasureChest() = default;

public:
    void update(float delta_time) override;

    void draw() const override;

public:
	void message(const std::string& message, std::any& param) override;

	bool can_interact() const override;

	std::string interact_text() const override;

private:
    void open();

private:
    ItemData::Data generate_data_;

    bool is_open_{ false };

};

#endif
