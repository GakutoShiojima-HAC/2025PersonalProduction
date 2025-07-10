// -----------------------------------------------------------------------------------------
//  File          : Assets.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/07
//  Updated       : 2025/07/11
//  Description   : �A�Z�b�g�ԍ��̒�`�A������ϊ��N���X
//					��`��������������̂܂�enum�ɕϊ�����̂ŁAmap����炸��`�ł���
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef ASSETS_H_
#define ASSETS_H_

#pragma region [��`]
// ���b�V���E�X�L�����b�V��
#define MESH_ID_LIST \
    X(NONE) \
    X(Player) \
    X(Enemy) \
    X(MAX_SIZE) // �v�f��

// �e�N�X�`�� 
#define TEXTURE_ID_LIST \
    X(NONE) \
    X(Skybox) \
    X(MAX_SIZE) // �v�f��

// �I�N�c���[
#define OCTREE_ID_LIST \
    X(NONE) \
    X(Mesh) \
    X(Collider) \
    X(MAX_SIZE) // �v�f��

// BGM
#define BGM_ID_LIST \
    X(NONE) \
    X(Title) \
    X(MAX_SIZE) // �v�f��

// SE
#define SE_ID_LIST \
    X(NONE) \
    X(Click) \
    X(MAX_SIZE) // �v�f��

// �G�t�F�N�g
#define EFFECT_ID_LIST \
    X(NONE) \
    X(Boom) \
    X(MAX_SIZE) // �v�f��
#pragma endregion

#include <string>
#include <unordered_map>

typedef	unsigned int GSuint;
using namespace std;

#pragma region [�񋓑̂𐶐�]
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
    /// ����������b�V���ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_mesh_id(const string& name) {
        static const unordered_map<string, MeshID> map = {
    #define X(name) {#name, MeshID::name},
            MESH_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)MeshID::NONE;
    }

    /// <summary>
    /// ��������e�N�X�`���ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_texture_id(const string& name) {
        static const unordered_map<string, TextureID> map = {
    #define X(name) {#name, TextureID::name},
            TEXTURE_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)TextureID::NONE;
    }

    /// <summary>
    /// �������n�`�f�[�^�ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_octree_id(const string& name) {
        static const unordered_map<string, OctreeID> map = {
    #define X(name) {#name, OctreeID::name},
            OCTREE_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)OctreeID::NONE;
    }

    /// <summary>
    /// �������BGM�ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_bgm_id(const string& name) {
        static const unordered_map<string, BGMID> map = {
    #define X(name) {#name, BGMID::name},
            BGM_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)BGMID::NONE;
    }

    /// <summary>
    /// �������SE�ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_se_id(const string& name) {
        static const unordered_map<string, SEID> map = {
    #define X(name) {#name, SEID::name},
            SE_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)SEID::NONE;
    }

    /// <summary>
    /// ��������G�t�F�N�g�ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_effect_id(const string& name) {
        static const unordered_map<string, EffectID> map = {
    #define X(name) {#name, EffectID::name},
            EFFECT_ID_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)EffectID::NONE;
    }
}

#endif
