#include "Engine/Core/Assets/AssetsManager.h"
#include <gslib.h>
#include <GSeffect.h>

#define GS_ENABLE_MESH_SHADOW			// メッシュに影を付ける
//#define GS_ENABLE_SKIN_MESH_SHADOW	// スキニングメッシュに影を付ける
#define GS_ENABLE_SOFT_SHADOW			// 影の輪郭をぼかす
#include <GSstandard_shader.h>

AssetsManager::~AssetsManager() {
	clear();
}

AssetsManager& AssetsManager::get_instance() {
	// static変数のインスタンスは１つ
	// インスタンス化も１回のみ
	static AssetsManager self;
	return self;
}

void AssetsManager::clear() {
	// 読み込み済みアセットを破棄
	for (auto& asset : assets_) {
        assets_delete(asset);    // 破棄
	}

    // リストをクリア
    for (auto asset : assets_) {
        delete asset;
        asset = nullptr;
    }
	assets_.clear();

}

LoadedAssets* AssetsManager::load_asset(LoadAssets* data) {
	LoadedAssets* result = new LoadedAssets{};
    if (data == nullptr) return result;

    gsInitDefaultShader();
	result->name = data->name;

	for (const auto& asset : data->mesh) {
		gsLoadMesh(asset.first, asset.second.c_str());
		result->mesh.push_back(asset.first);
	}
	for (const auto& asset : data->skinmesh) {
		gsLoadSkinMesh(asset.first, asset.second.c_str());
		result->skinmesh.push_back(asset.first);
	}
	for (const auto& asset : data->octree) {
		gsLoadOctree(asset.first, asset.second.c_str());
		result->octree.push_back(asset.first);
	}
	for (const auto& asset : data->texture) {
		gsLoadTexture(asset.first, asset.second.c_str());
		result->texture.push_back(asset.first);
	}
	for (const auto& asset : data->bgm) {
		gsLoadBGM(asset.first, asset.second.c_str(), GS_TRUE);
		result->bgm.push_back(asset.first);
	}
	for (const auto& asset : data->se) {
		gsLoadSE(asset.first, asset.second.c_str(), 5, GWAVE_DEFAULT);
		result->se.push_back(asset.first);
	}
	for (const auto& asset : data->effect) {
		gsLoadEffect(asset.first, asset.second.c_str());
		result->effect.push_back(asset.first);
	}

	assets_.push_back(result);  // 管理下に追加

    delete data;                // 不要なので削除
    data = nullptr;

	return result;
}

bool AssetsManager::delete_asset(const string& assets_name) {
	return delete_asset(find(assets_name));
}

bool AssetsManager::delete_asset(LoadedAssets* data) {
    if (data == nullptr) return false;
    assets_delete(data);    // 破棄
	remove(data);	// 管理下から削除
	return true;
}

LoadedAssets* AssetsManager::find(const string& assets_name) {
	for (const auto& asset : assets_) {
		if (asset->name == assets_name) {
			return  asset;
		}
	}
	return nullptr;
}

void AssetsManager::assets_delete(LoadedAssets* data) {
    if (data == nullptr) return;

    for (const auto& asset : data->mesh) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->mesh)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteMesh(asset);
    }
    for (const auto& asset : data->skinmesh) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->skinmesh)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteSkinMesh(asset);
    }
    for (const auto& asset : data->octree) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->octree)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteOctree(asset);
    }
    for (const auto& asset : data->texture) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->texture)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteTexture(asset);
    }
    for (const auto& asset : data->bgm) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->bgm)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteBGM(asset);
    }
    for (const auto& asset : data->se) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->se)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteSE(asset);
    }
    for (const auto& asset : data->effect) {
        bool same{ false };
        for (const auto& other : assets_) {
            if (other == data) continue;
            if (is_same_id_for_other_asset(asset, other->effect)) {
                same = true;
                break;
            }
        }
        if (!same) gsDeleteEffect(asset);
    }
}

void AssetsManager::remove(LoadedAssets* data) {
    auto it = std::find(assets_.begin(), assets_.end(), data);
    if (it != assets_.end()) {
        delete* it;
        assets_.erase(it);
    }
}

bool AssetsManager::is_same_id_for_other_asset(const GSuint id, const vector<GSuint>& other) const {
	for (const auto& other_id : other) {
		if (id == other_id) return true;
	}
	return false;
}
