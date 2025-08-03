// -----------------------------------------------------------------------------------------
//  File        : Folder.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/08/01
//  Updated     : 2025/08/01
//  Description : �t�H���_�E�t�@�C���p�֗��֐��Q
//
//  ���ӁF�{�\�[�X�R�[�h�̖��f�]�ځE�R�[�h�̃R�s�[�E�\��t���ɂ�闬�p�E�Ĕz�z���֎~���܂��B
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_FOLDER_H_
#define MYLIB_FOLDER_H_

#include <string>
#include <fstream>
#include <cstring>
#include <sys/stat.h>    // mkdir�p
#include <sys/types.h>   // mkdir�p
#ifdef _WIN32
#include <direct.h>      // Windows�p
#define mkdir(dir, mode) _mkdir(dir) // Windows�̓��[�h�𖳎����邽��
#endif
#include "Lib/json.hpp"

using namespace std;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace MyLib {
    /// <summary>
    /// �t�H���_���쐬
    /// </summary>
    /// <param name="path">= �t�H���_�܂ł̃p�X���w�� �e�t�H���_��������ΐe���쐬����</param>
    inline bool create_folder(const string& path) {
        size_t pos = 0;
        string current_path;

        // ���[�g���珇�Ƀp�X���쐬
        while ((pos = path.find_first_of("/\\", pos)) != string::npos) {
            current_path = path.substr(0, pos);
            ++pos; // ���̈ʒu��

            // �󕶎��̏ꍇ���X�L�b�v (�ŏ��̃X���b�V���Ȃ�)
            if (current_path.empty()) continue;

            // �t�H���_�����݂��邩�m�F
            struct stat info;
            if (stat(current_path.c_str(), &info) != 0) {
                // �t�H���_�����݂��Ȃ��ꍇ�͍쐬
                if (mkdir(current_path.c_str(), 0755) != 0) {
                    return false;
                }
            }
            else if (!(info.st_mode & S_IFDIR)) {
                // �쐬�������t�H���_�Ɠ����̃t�@�C�������݂���ꍇ�̓G���[
                return false;
            }
        }

        // �Ō�̃f�B���N�g�����쐬 (�t���p�X�̏ꍇ)
        struct stat info;
        if (stat(path.c_str(), &info) != 0) {
            if (mkdir(path.c_str(), 0755) != 0) {
                return false;
            }
        }

        return true;
    }

    /// <summary>
    /// �t�@�C�����쐬���ď㏑���ۑ�����
    /// </summary>
    /// <param name="path">= �t�@�C���ւ̃t���p�X �g���q�܂ŕK�v</param>
    /// <param name="content">= �������ޓ��e</param>
    inline bool write_to_file(const string& path, const string& content) {
        ofstream f(path, ios::out);   // �㏑����ios::out
        if (!f) return false;

        f << content;
        f.close();
        return true;
    }

    /// <summary>
    /// json�t�@�C�����쐬���ď㏑���ۑ�����
    /// </summary>
    /// <param name="j">= json�t�@�C���ւ̃t���p�X �g���q�܂ŕK�v</param>
    /// <param name="content">= �������ޓ��e</param>
    inline bool write_to_file(const string& path, const ordered_json& j) {
        ofstream f(path, ios::out);   // �㏑����ios::out
        if (!f) return false;

        f << j.dump(4);  // �C���f���g
        f.close();
        return true;
    }

    /// <summary>
    /// json�t�@�C�����쐬���ď㏑���ۑ�����
    /// </summary>
    /// <param name="j">= json�t�@�C���ւ̃t���p�X �g���q�܂ŕK�v</param>
    /// <param name="content">= �������ޓ��e</param>
    inline bool write_to_file(const string& path, const json& j) {
        ofstream f(path, ios::out);   // �㏑����ios::out
        if (!f) return false;

        f << j.dump(4);  // �C���f���g
        f.close();
        return true;
    }

    /// <summary>
    /// �t�@�C�����쐬���ĒǋL�ۑ�����
    /// </summary>
    /// <param name="path">= �t�@�C���ւ̃t���p�X �g���q�܂ŕK�v</param>
    /// <param name="content">= �������ޓ��e</param>
    inline bool append_to_file(const string& path, const string& content) {
        ofstream f(path, ios::app);   // �ǋL��ios::app
        if (!f) return false;

        f << content;
        f.close();
        return true;
    }

    /// <summary>
    /// json�t�@�C�����쐬���ĒǋL�ۑ�����
    /// </summary>
    /// <param name="j">= json�t�@�C���ւ̃t���p�X �g���q�܂ŕK�v</param>
    /// <param name="content">= �������ޓ��e</param>
    inline bool append_to_file(const string& path, const json& j) {
        ofstream f(path, ios::app);   // �ǋL��ios::app
        if (!f) return false;

        f << j.dump(4);  // �C���f���g
        f.close();
        return true;
    }

    /// <summary>
    /// json�t�@�C�����쐬���ĒǋL�ۑ�����
    /// </summary>
    /// <param name="j">= json�t�@�C���ւ̃t���p�X �g���q�܂ŕK�v</param>
    /// <param name="content">= �������ޓ��e</param>
    inline bool append_to_file(const string& path, const ordered_json& j) {
        ofstream f(path, ios::app);   // �ǋL��ios::app
        if (!f) return false;

        f << j.dump(4);  // �C���f���g
        f.close();
        return true;
    }
}

#endif
