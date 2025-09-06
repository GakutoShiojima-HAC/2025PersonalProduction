#ifndef WEAPON_TYPE_H_
#define WEAPON_TYPE_H_

#define WEAPON_TYPE_LIST \
    X(NONE) \
    X(PlayerSword) \
    X(MAX_SIZE) // —v‘f”

#include <string>
#include <unordered_map>

typedef	unsigned int GSuint;
using namespace std;

#define X(name) name,
enum class WeaponType { WEAPON_TYPE_LIST };
#undef X

namespace Weapons {
    /// <summary>
    /// •¶š—ñ‚ğ•Ší”Ô†‚É•ÏŠ·
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