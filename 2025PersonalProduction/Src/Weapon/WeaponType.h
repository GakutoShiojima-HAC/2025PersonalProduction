// -----------------------------------------------------------------------------------------
//  File          : WeaponType.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/09/06
//  Updated       : 2025/09/07
//  Description   : ����̎�ނ��`
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef WEAPON_TYPE_H_
#define WEAPON_TYPE_H_

#define WEAPON_TYPE_LIST \
    X(NONE) \
    X(PlayerSword) \
    X(MAX_SIZE) // �v�f��

#include <string>
#include <unordered_map>

typedef	unsigned int GSuint;
using namespace std;

#define X(name) name,
enum class WeaponType { WEAPON_TYPE_LIST };
#undef X

namespace Weapons {
    /// <summary>
    /// ������𕐊�ԍ��ɕϊ�
    /// </summary>
    inline GSuint to_weapon_id(const string& name) {
        static const unordered_map<string, WeaponType> map = {
    #define X(name) {#name, WeaponType::name},
            WEAPON_TYPE_LIST
    #undef X
        };
        auto it = map.find(name);
        return (it != map.end()) ? (GSuint)it->second : (GSuint)WeaponType::NONE;
    }
}

#endif