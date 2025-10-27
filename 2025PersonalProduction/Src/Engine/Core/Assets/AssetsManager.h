// -----------------------------------------------------------------------------------------
//  File          : AssetsManager.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/11
//  Updated       : 2025/09/29
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

// 読み込むアセット
struct LoadAssets {
	std::string name{ "" };	// 識別名
	vector<pair<GSuint, std::string>> mesh;
	vector<pair<GSuint, std::string>> skinmesh;
	vector<pair<GSuint, std::string>> texture;
	vector<pair<GSuint, std::string>> octree;
	vector<pair<GSuint, std::string>> bgm;
	vector<pair<GSuint, std::string>> se;
	vector<pair<GSuint, std::string>> effect;
    std::string light_map;
    std::string reflection_probe;
};

// 読み込んだアセット
struct LoadedAssets {
	std::string name{ "" };	// 識別名
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
	LoadedAssets* load_asset(LoadAssets* data);

	/// <summary>
	/// 指定したアセットを破棄
	/// </summary>
	/// <param name="name">= 識別名</param>
	/// <returns>破棄が完了したら真</returns>
	bool delete_asset(const std::string& assets_name);

	/// <summary>
	/// 指定したアセットを破棄
	/// </summary>
	/// <param name="data">= データ</param>
	/// <returns>破棄が完了したら真</returns>
	bool delete_asset(LoadedAssets* data);

	/// <summary>
	/// 識別名から読み込み済みアセット構造体を検索
	/// </summary>
	/// <param name="name">= 識別名</param>
	/// <returns>構造体</returns>
	LoadedAssets* find(const std::string& assets_name);

private:
    /// <summary>
    /// アセットを破棄する
    /// </summary>
    /// <param name="data">= 読み込み済みデータ</param>
    void assets_delete(LoadedAssets* data);

	/// <summary>
	/// 指定した読み込み済みアセットを管理下から消去する
	/// </summary>
	/// <param name="data">= データ</param>
	void remove(LoadedAssets* data);

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

public:
	// コピー禁止
	AssetsManager(const AssetsManager& other) = delete;
	AssetsManager& operator = (const AssetsManager& other) = delete;

};

#endif
