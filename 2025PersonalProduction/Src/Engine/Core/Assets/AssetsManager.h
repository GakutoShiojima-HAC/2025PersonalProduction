// -----------------------------------------------------------------------------------------
//  File          : AssetsManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/11
//  Updated       : 2025/07/11
//  Description   : アセットのロード・クリアを管理するクラス
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef ASSETS_MANAGER_H_
#define ASSETS_MANAGER_H_

#include <string>
#include <vector>
#include <list>
#include <utility>

typedef	unsigned int GSuint;
using namespace std;

// 読み込むアセット
struct LoadAssets {
	string name{ "" };	// 識別名
	vector<pair<GSuint, string>> mesh;
	vector<pair<GSuint, string>> skinmesh;
	vector<pair<GSuint, string>> texture;
	vector<pair<GSuint, string>> octree;
	vector<pair<GSuint, string>> bgm;
	vector<pair<GSuint, string>> se;
	vector<pair<GSuint, string>> effect;
};

// 読み込んだアセット
struct LoadedAssets {
	string name{ "" };	// 識別名
	vector<GSuint> mesh;
	vector<GSuint> skinmesh;
	vector<GSuint> texture;
	vector<GSuint> octree;
	vector<GSuint> bgm;
	vector<GSuint> se;
	vector<GSuint> effect;
};

class AssetsManager {
private:
	// コンストラクタ（外部からのインスタンスを禁止）
	AssetsManager() = default;

public:
	~AssetsManager();

public:
	/// <summary>
	/// インスタンスを取得するためのstaticメンバ関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static AssetsManager& get_instance();

public:
	/// <summary>
	/// 管理下のアセットを全て破棄し、管理下から消去
	/// </summary>
	void clear();

	/// <summary>
	/// アセットをロード
	/// </summary>
	/// <param name="data">= データ</param>
	/// <returns>データ</returns>
	LoadedAssets* load_assets(LoadAssets* data);

	/// <summary>
	/// アセットを非同期でロード
	/// </summary>
	/// <param name="data">= データ</param>
	void load_assets_async(LoadAssets* data);

public:
	/// <summary>
	/// 指定したアセットを破棄
	/// </summary>
	/// <param name="name">= 識別名</param>
	/// <returns>破棄が完了したら真</returns>
	bool delete_assets(const string& assets_name);

	/// <summary>
	/// 指定したアセットを破棄
	/// </summary>
	/// <param name="data">= データ</param>
	/// <returns>破棄が完了したら真</returns>
	bool delete_assets(LoadedAssets* data);

public:
	/// <summary>
	/// 非同期でアセットを読み込んでいるか
	/// </summary>
	/// <returns>していたら真</returns>
	bool is_async_load() const;

	/// <summary>
	/// 読み込んだアセット数 読み込み終了で0にリセット
	/// </summary>
	/// <returns>数</returns>
	int loaded_count() const;

	/// <summary>
	/// 識別名から読み込み済みアセット構造体を検索
	/// </summary>
	/// <param name="name">= 識別名</param>
	/// <returns>構造体</returns>
	LoadedAssets* find(const string& assets_name);

private:
	/// <summary>
	/// 非同期でアセット読み込みを開始
	/// </summary>
	/// <param name="data">データ</param>
	void async_load_asset(LoadAssets* data);
	
	/// <summary>
	/// アセットを破棄
	/// </summary>
	/// <param name="data">= 破棄するアセットデータ</param>
	void delete_asset(LoadedAssets* data);

	/// <summary>
	/// 指定した読み込み済みアセットを管理下から消去する
	/// </summary>
	/// <param name="data">= データ</param>
	void remove_loaded_asset(LoadedAssets* data);

	/// <summary>
	/// 指定した読み込み中アセットを管理下から消去する
	/// </summary>
	/// <param name="data">= データ</param>
	void remove_loading_asset(LoadAssets* data);

	/// <summary>
	/// 同じリソースIDを他アセットでも使用しているか
	/// </summary>
	/// <param name="id">= 確認したいID</param>
	/// <param name="other">= 同種リソース</param>
	/// <returns>使用していたら真</returns>
	bool is_same_id_for_other_asset(const GSuint id, const vector<GSuint>& other) const;

private:
	// 読み込んだアセット
	list<LoadedAssets*> assets_;
	// 非同期で読み込み中のアセット
	list<LoadAssets*> loading_assets_;

	// 読み込んだアセット数
	int loaded_assets_count_{ 0 };

public:
	// コピー禁止
	AssetsManager(const AssetsManager& other) = delete;
	AssetsManager& operator = (const AssetsManager& other) = delete;

};

#endif
