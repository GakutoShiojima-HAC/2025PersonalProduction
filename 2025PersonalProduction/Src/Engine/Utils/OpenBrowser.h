// -----------------------------------------------------------------------------------------
//  File          : OpenBrowser.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : ブラウザを開く汎用関数をまとめたクラス
//                  名前空間はMyLibを使用している
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef OPEN_BROWSER_H_
#define OPEN_BROWSER_H_

#include <string>

using namespace std;

namespace MyLib {
	/// <summary>
	/// URLからブラウザを開く
	/// </summary>
	/// <param name="url">= ページのURL</param>
	inline void open_browser_by_url(const char* url) {
#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
        ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);

#elif defined(__APPLE__)
#include <cstdlib>
        std::string command = "open ";
        command += url;
        system(command.c_str());

#elif defined(__linux__)
#include <cstdlib>
        std::string command = "xdg-open ";
        command += url;
        system(command.c_str());
#endif
	}
}

#endif
