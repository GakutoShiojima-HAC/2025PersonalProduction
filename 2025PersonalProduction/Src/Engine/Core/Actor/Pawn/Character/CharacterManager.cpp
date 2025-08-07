#include "Engine/Core/Actor/Pawn/Character/CharacterManager.h"
#include "Engine/Core/Actor/Pawn/Character/Character.h"

CharacterManager::~CharacterManager() {
    clear();
}

void CharacterManager::add(Character* character) {
    characters_.push_back(character);
}

void CharacterManager::remove() {
    for (auto i = characters_.begin(); i != characters_.end(); ) {
        if ((*i)->is_dead()) {
            i = characters_.erase(i);
        }
        else {
            ++i;
        }
    }
}

void CharacterManager::clear() {
    characters_.clear();
}

Character* CharacterManager::find(const string& name) const {
    for (const auto& character : characters_) {
        if (character->name() == name) {
            return character;
        }
    }
    return nullptr;
}

vector<Character*> CharacterManager::find_with_tag(const ActorTag tag) const {
    vector<Character*> result;
    for (const auto& character : characters_) {
        if (character->tag() == tag) {
            result.push_back(character);
        }
    }
    return result;
}

int CharacterManager::count() const {
    return characters_.size();
}

int CharacterManager::count_with_tag(const ActorTag tag) const {
    int count{ 0 };
    for (const auto& character : characters_) {
        if (character->tag() == tag) {
            ++count;
        }
    }

    return count;
}
