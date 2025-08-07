#include "Engine/Core/Actor/Pawn/PawnManager.h"
#include "Engine/Core/Actor/Pawn/Pawn.h"

PawnManager::~PawnManager() {
	clear();
}

void PawnManager::add(Pawn* pawn) {
	pawns_.push_back(pawn);
}

void PawnManager::remove() {
    for (auto i = pawns_.begin(); i != pawns_.end(); ) {
        if ((*i)->is_dead()) {
            i = pawns_.erase(i);
        }
        else {
            ++i;
        }
    }
}

void PawnManager::clear() {
    pawns_.clear();
}

Pawn* PawnManager::find(const string& name) const {
    for (const auto& pawn : pawns_) {
        if (pawn->name() == name) {
            return pawn;
        }
    }
    return nullptr;
}

vector<Pawn*> PawnManager::find_with_tag(const ActorTag tag) const {
    vector<Pawn*> result;
    for (const auto& pawn : pawns_) {
        if (pawn->tag() == tag) {
            result.push_back(pawn);
        }
    }
    return result;
}

int PawnManager::count() const {
    return pawns_.size();
}

int PawnManager::count_with_tag(const ActorTag tag) const {
    int count{ 0 };
    for (const auto& pawn : pawns_) {
        if (pawn->tag() == tag) {
            ++count;
        }
    }

    return count;
}
