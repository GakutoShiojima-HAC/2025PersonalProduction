// -----------------------------------------------------------------------------------------
//  File        : SystemLog.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/09/15
//  Updated     : 2025/09/15
//  Description : システムデータのログを出力する
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------


#ifndef SYSTEM_LOG_H_
#define SYSTEM_LOG_H_

#include <string>

namespace LogSystem {
    namespace System {
        struct Data {
            // アプリケーション開始時刻
            std::string start_time{ "" };
            // アプリケーション終了時刻
            std::string end_time{ "" };

            // シード値
            unsigned int seed{ 0 };
        };

        /// <summary>
        /// ログの保存
        /// </summary>
        /// <param name="folder_path">= 出力先のフォルダ</param>
        /// <param name="data">= データ</param>
        void save(const std::string& folder_path, Data* data);
    }
}

#endif
