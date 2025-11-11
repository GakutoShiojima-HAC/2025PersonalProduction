// -----------------------------------------------------------------------------------------
//  File          : ActorTag.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : アクタータグ定義クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ACTOR_TAG_H_
#define ACTOR_TAG_H_

enum class ActorTag {
	None,
	Player,
	Enemy,
	Collider,
    Item,
    Object,

	MAX_SIZE	// 要素数
};

#endif
