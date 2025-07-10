#include "Engine/Core/Assets/AssetsManager.h"
#include <gslib.h>
#include <GSeffect.h>

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
	// 読み込み中の構造体を破棄
	for (auto asset : loading_assets_) {
		delete asset;
	}
	loading_assets_.clear();

	// 読み込み済みアセットを破棄
	for (auto asset : assets_) {
		delete_asset(asset);
		delete asset;
	}
	assets_.clear();
}

LoadedAssets* AssetsManager::load_assets(LoadAssets* data) {
	LoadedAssets* result = new LoadedAssets{};
	result->name = data->name;
	int loaded_assets_num{ 0 };

	// ロード終了した数をカウント
	auto countup = [this, &loaded_assets_num]() {
		++loaded_assets_num;
		++loaded_assets_count_;
	};

	for (const auto& asset : data->mesh) {
		gsLoadMesh(asset.first, asset.second.c_str());
		countup();
		result->mesh.push_back(asset.first);
	}
	for (const auto& asset : data->skinmesh) {
		gsLoadSkinMesh(asset.first, asset.second.c_str());
		countup();
		result->skinmesh.push_back(asset.first);
	}
	for (const auto& asset : data->octree) {
		gsLoadOctree(asset.first, asset.second.c_str());
		countup();
		result->octree.push_back(asset.first);
	}
	for (const auto& asset : data->texture) {
		gsLoadTexture(asset.first, asset.second.c_str());
		countup();
		result->texture.push_back(asset.first);
	}
	for (const auto& asset : data->mesh) {
		gsLoadBGM(asset.first, asset.second.c_str(), GS_TRUE);
		countup();
		result->bgm.push_back(asset.first);
	}
	for (const auto& asset : data->texture) {
		gsLoadSE(asset.first, asset.second.c_str(), 5, GWAVE_DEFAULT);
		countup();
		result->se.push_back(asset.first);
	}
	for (const auto& asset : data->texture) {
		gsLoadEffect(asset.first, asset.second.c_str());
		countup();
		result->effect.push_back(asset.first);
	}
	// ロードが終了したので数をリセット
	loaded_assets_count_ -= loaded_assets_num;
	delete data;
	assets_.push_back(result);
	return result;
}

void AssetsManager::load_assets_async(LoadAssets* data) {
	loading_assets_.push_back(data);	// 読み込み中に追加
	// 別スレッドで読み込み処理を行う
	gslib::Game::run_thread([=] { async_load_asset(data); });
}

bool AssetsManager::delete_assets(const string& assets_name) {
	return delete_assets(find(assets_name));
}

bool AssetsManager::delete_assets(LoadedAssets* data) {
	if (data == nullptr) return false;
	delete_asset(data);
	remove_loaded_asset(data);	// 管理下から消去
	return true;
}

bool AssetsManager::is_async_load() const {
	return loading_assets_.size() > 0;
}

int AssetsManager::loaded_count() const {
	return loaded_assets_count_;
}

LoadedAssets* AssetsManager::find(const string& assets_name) {
	for (const auto& asset : assets_) {
		if (asset->name == assets_name) {
			return  asset;
		}
	}
	return nullptr;
}

void AssetsManager::async_load_asset(LoadAssets* data) {
	load_assets(data);				// 読み込む
	remove_loading_asset(data);		// 読み込み中から消去
}

void AssetsManager::delete_asset(LoadedAssets* data) {
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

void AssetsManager::remove_loaded_asset(LoadedAssets* data) {
	for (auto i = assets_.begin(); i != assets_.end();) {
		if ((*i) == data) {
			delete* i;
			i = assets_.erase(i);
		}
		else {
			++i;
		}
	}
}

void AssetsManager::remove_loading_asset(LoadAssets* data) {
	for (auto i = loading_assets_.begin(); i != loading_assets_.end();) {
		if ((*i) == data) {
			delete* i;
			i = loading_assets_.erase(i);
		}
		else {
			++i;
		}
	}
}

bool AssetsManager::is_same_id_for_other_asset(const GSuint id, const vector<GSuint>& other) const {
	for (const auto& other_id : other) {
		if (id == other_id) return true;
	}
	return false;
}
