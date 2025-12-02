// -----------------------------------------------------------------------------------------
//  File          : StaticEffect.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/11/28
//  Updated       : 2025/11/28
//  Description   : ワールドに出す、動かない演出用エフェクト
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef STATICEFFECT_H_
#define STATICEFFECT_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <GSvector3.h>

class StaticEffect {
public:
	StaticEffect() = default;

	~StaticEffect() = default;

public:
    /// <summary>
    /// 配置データの読み込み
    /// </summary>
    /// <param name="json_path">= データパス</param>
    void load(const std::string& json_path);

    /// <summary>
    /// 生成
    /// </summary>
    void generate();

    /// <summary>
    /// 破棄
    /// </summary>
    void clear();

private:
    // 生成座標
    std::unordered_map<unsigned int, std::vector<GSvector3>> positions_;
    // エフェクトハンドル
    std::vector<int> handles_;

public:
	// コピー禁止
	StaticEffect(const StaticEffect& other) = delete;
	StaticEffect& operator = (const StaticEffect& other) = delete;

};

#endif
