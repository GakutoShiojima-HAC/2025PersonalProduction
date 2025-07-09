// -----------------------------------------------------------------------------------------
//  File          : OpenBrowser.h
//  Author        : Shiojima Gakuto
//  Created       : 2025/07/09
//  Updated       : 2025/07/09
//  Description   : �u���E�U���J���ėp�֐����܂Ƃ߂��N���X
//                  ���O��Ԃ�MyLib���g�p���Ă���
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef OPEN_BROWSER_H_
#define OPEN_BROWSER_H_

#include <string>

using namespace std;

namespace MyLib {
	/// <summary>
	/// URL����u���E�U���J��
	/// </summary>
	/// <param name="url">= �y�[�W��URL</param>
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
