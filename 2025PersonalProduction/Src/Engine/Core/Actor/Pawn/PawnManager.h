// -----------------------------------------------------------------------------------------
//  File          : PawnManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : ポーン管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef PAWN_MANAGER_H_
#define PAWN_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Actor/ActorTag.h"

using namespace std;

class Pawn;

class PawnManager {
public:
	PawnManager() = default;

	~PawnManager();

public:
	/// <summary>
	/// ポーンを管理下に追加
	/// </summary>
	/// <param name="pawn">= ポーン</param>
	void add(Pawn* pawn);

	/// <summary>
	/// 寿命が尽きたポーンの削除
	/// </summary>
	void remove();

	/// <summary>
	/// 管理下から全てのポーンを削除
	/// </summary>
	void clear();

public:
    /// <summary>
    /// 指定した名前を持つポーンを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればポーンを、見つからなければnullptr</returns>
    Pawn* find(const string& name) const;

    /// <summary>
    /// 指定したタグを持つポーンを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>ポーン</returns>
    vector<Pawn*> find_with_tag(const ActorTag tag) const;

    /// <summary>
    /// 管理しているポーンの数
    /// </summary>
    /// <returns>数</returns>
    int count() const;

    /// <summary>
    /// 指定したタグを持つポーンの数
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>数</returns>
    int count_with_tag(const ActorTag tag) const;

private:
    list<Pawn*> pawns_;

public:
	// コピー禁止
	PawnManager(const PawnManager& other) = delete;
	PawnManager& operator = (const PawnManager& other) = delete;

};

#endif
