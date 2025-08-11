// -----------------------------------------------------------------------------------------
//  File          : CharacterManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/08/08
//  Updated       : 2025/08/08
//  Description   : キャラクター管理クラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef CHARACTER_MANAGER_H_
#define CHARACTER_MANAGER_H_

#include <list>
#include <vector>
#include <string>
#include "Actor/ActorTag.h"

using namespace std;

class Character;

class CharacterManager {
public:
    CharacterManager() = default;

    ~CharacterManager();

public:
    /// <summary>
    /// キャラクターを管理下に追加
    /// </summary>
    /// <param name="character">= キャラクター</param>
    void add(Character* character);

    /// <summary>
    /// 寿命が尽きたキャラクターの削除
    /// </summary>
    void remove();

    /// <summary>
    /// 管理下から全てのキャラクターを削除
    /// </summary>
    void clear();

public:
    /// <summary>
    /// 指定した名前を持つキャラクターを検索
    /// </summary>
    /// <param name="name">= 名前</param>
    /// <returns>見つかればキャラクターを、見つからなければnullptr</returns>
    Character* find(const string& name) const;

    /// <summary>
    /// 指定したタグを持つキャラクターを全て検索
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>キャラクター</returns>
    vector<Character*> find_with_tag(const ActorTag tag) const;

    /// <summary>
    /// 管理しているキャラクターの数
    /// </summary>
    /// <returns>数</returns>
    int count() const;

    /// <summary>
    /// 指定したタグを持つキャラクターの数
    /// </summary>
    /// <param name="tag">= タグ</param>
    /// <returns>数</returns>
    int count_with_tag(const ActorTag tag) const;

private:
    list<Character*> characters_;

public:
    // コピー禁止
    CharacterManager(const CharacterManager& other) = delete;
    CharacterManager& operator = (const CharacterManager& other) = delete;

};

#endif
