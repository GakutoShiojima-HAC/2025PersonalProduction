// -----------------------------------------------------------------------------------------
//  File          : Assets.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/07
//  Updated       : 2025/10/29
//  Description   : アセット番号の定義、文字列変換クラス
//					定義した文字列をそのままenumに変換するので、mapを作らず定義できる
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ASSETS_H_
#define ASSETS_H_

#pragma region [定義]
// メッシュ・スキンメッシュ
#define MESH_ID_LIST \
    X(NONE) \
    X(Player) \
    X(DummyEnemy) \
    X(EnemySlimeToxic) \
    X(EnemySlimeManToxic) \
    X(EnemyRockGolemMud) \
    X(EnemyLich) \
    X(GimmickBarrier) \
    X(GimmickElevator) \
    X(WeaponSword1) \
    X(WeaponSword2) \
    X(MAX_SIZE) // 要素数

// テクスチャ 
#define TEXTURE_ID_LIST \
    X(NONE) \
    X(Cursor) \
    X(MenuWindowTexture) \
    X(PauseWindowTexture) \
    X(SettingWindowTexture) \
    X(GameGuideWindowTexture) \
    X(ButtonSelectMouseTexture) \
    X(ButtonSelectGamePadTexture) \
    X(GearIconTexture) \
    X(GamePadIconTexture) \
    X(ArrowIconTexture) \
    X(DotIconTexture) \
    X(Skybox) \
    X(TitleLogo) \
    X(MenuLogo) \
    X(SafeArea) \
    X(BonusGauge) \
    X(HPGauge) \
    X(HPGaugeBG) \
    X(HPBossGauge) \
    X(HPBossGaugeBG) \
    X(StatusGaugeBG) \
    X(MAX_SIZE) // 要素数

// オクツリー
#define OCTREE_ID_LIST \
    X(NONE) \
    X(Mesh) \
    X(Collider) \
    X(MAX_SIZE) // 要素数

// BGM
#define BGM_ID_LIST \
    X(NONE) \
    X(Title) \
    X(GameNormal) \
    X(GameBattle) \
    X(GameBoss) \
    X(MAX_SIZE) // 要素数

// SE
#define SE_ID_LIST \
    X(NONE) \
    X(AddScore) \
    X(MAX_SIZE) // 要素数

// エフェクト
#define EFFECT_ID_LIST \
    X(NONE) \
    X(Avoid) \
    X(OnGroundSmoke) \
    X(DangerSignal) \
    X(Item) \
    X(HitAttack) \
    X(PlayerSlash) \
    X(DustLarge) \
    X(FireBall) \
    X(ExplosionSmall) \
    X(MAX_SIZE) // 要素数
#pragma endregion

#include <string>
#include <unordered_map>

typedef	unsigned int GSuint;
using namespace std;

#pragma region [列挙体を生成]
#define X(name) name,
enum class MeshID { MESH_ID_LIST };
enum class TextureID { TEXTURE_ID_LIST };
enum class OctreeID { OCTREE_ID_LIST };
enum class BGMID { BGM_ID_LIST };
enum class SEID { SE_ID_LIST };
enum class EffectID { EFFECT_ID_LIST };
#undef X
#pragma endregion

namespace Assets {
    /// <summary>
    /// 文字列をメッシュ番号に変換
    /// </summary>
    inline GSuint to_mesh_id(const string& name) {
        static const unordered_map<string, MeshID> map = {
    #define X(name) { #name, MeshID::name },
            MESH_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)MeshID::NONE;
    }

    /// <summary>
    /// 文字列をテクスチャ番号に変換
    /// </summary>
    inline GSuint to_texture_id(const string& name) {
        static const unordered_map<string, TextureID> map = {
    #define X(name) { #name, TextureID::name },
            TEXTURE_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)TextureID::NONE;
    }

    /// <summary>
    /// 文字列を地形データ番号に変換
    /// </summary>
    inline GSuint to_octree_id(const string& name) {
        static const unordered_map<string, OctreeID> map = {
    #define X(name) { #name, OctreeID::name },
            OCTREE_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)OctreeID::NONE;
    }

    /// <summary>
    /// 文字列をBGM番号に変換
    /// </summary>
    inline GSuint to_bgm_id(const string& name) {
        static const unordered_map<string, BGMID> map = {
    #define X(name) { #name, BGMID::name },
            BGM_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)BGMID::NONE;
    }

    /// <summary>
    /// 文字列をSE番号に変換
    /// </summary>
    inline GSuint to_se_id(const string& name) {
        static const unordered_map<string, SEID> map = {
    #define X(name) { #name, SEID::name },
            SE_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)SEID::NONE;
    }

    /// <summary>
    /// 文字列をエフェクト番号に変換
    /// </summary>
    inline GSuint to_effect_id(const string& name) {
        static const unordered_map<string, EffectID> map = {
    #define X(name) { #name, EffectID::name },
            EFFECT_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)EffectID::NONE;
    }
}

#endif
