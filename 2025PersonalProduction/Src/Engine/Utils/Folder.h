// -----------------------------------------------------------------------------------------
//  File        : Folder.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/08/01
//  Updated     : 2025/08/01
//  Description : フォルダ・ファイル用便利関数群
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_FOLDER_H_
#define MYLIB_FOLDER_H_

#include <string>
#include <fstream>
#include <cstring>
#include <sys/stat.h>    // mkdir用
#include <sys/types.h>   // mkdir用
#ifdef _WIN32
#include <direct.h>      // Windows用
#define mkdir(dir, mode) _mkdir(dir) // Windowsはモードを無視するため
#endif
#include "Lib/json.hpp"

#include <vector>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>  // ファイル検索用
namespace fs = std::experimental::filesystem;


using namespace std;
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

namespace MyLib {
    /// <summary>
    /// フォルダを作成
    /// </summary>
    /// <param name="path">= フォルダまでのパスを指定 親フォルダが無ければ親も作成する</param>
    inline bool create_folder(const string& path) {
        size_t pos = 0;
        string current_path;

        // ルートから順にパスを作成
        while ((pos = path.find_first_of("/\\", pos)) != string::npos) {
            current_path = path.substr(0, pos);
            ++pos; // 次の位置へ

            // 空文字の場合をスキップ (最初のスラッシュなど)
            if (current_path.empty()) continue;

            // フォルダが存在するか確認
            struct stat info;
            if (stat(current_path.c_str(), &info) != 0) {
                // フォルダが存在しない場合は作成
                if (mkdir(current_path.c_str(), 0755) != 0) {
                    return false;
                }
            }
            else if (!(info.st_mode & S_IFDIR)) {
                // 作成したいフォルダと同名のファイルが存在する場合はエラー
                return false;
            }
        }

        // 最後のディレクトリを作成 (フルパスの場合)
        struct stat info;
        if (stat(path.c_str(), &info) != 0) {
            if (mkdir(path.c_str(), 0755) != 0) {
                return false;
            }
        }

        return true;
    }

    /// <summary>
    /// ファイルを作成して上書き保存する
    /// </summary>
    /// <param name="path">= ファイルへのフルパス 拡張子まで必要</param>
    /// <param name="content">= 書き込む内容</param>
    inline bool write_to_file(const string& path, const string& content) {
        ofstream f(path, ios::out);   // 上書きはios::out
        if (!f) return false;

        f << content;
        f.close();
        return true;
    }

    /// <summary>
    /// jsonファイルを作成して上書き保存する
    /// </summary>
    /// <param name="j">= jsonファイルへのフルパス 拡張子まで必要</param>
    /// <param name="content">= 書き込む内容</param>
    inline bool write_to_file(const string& path, const ordered_json& j) {
        ofstream f(path, ios::out);   // 上書きはios::out
        if (!f) return false;

        f << j.dump(4);  // インデント
        f.close();
        return true;
    }

    /// <summary>
    /// jsonファイルを作成して上書き保存する
    /// </summary>
    /// <param name="j">= jsonファイルへのフルパス 拡張子まで必要</param>
    /// <param name="content">= 書き込む内容</param>
    inline bool write_to_file(const string& path, const json& j) {
        ofstream f(path, ios::out);   // 上書きはios::out
        if (!f) return false;

        f << j.dump(4);  // インデント
        f.close();
        return true;
    }

    /// <summary>
    /// ファイルを作成して追記保存する
    /// </summary>
    /// <param name="path">= ファイルへのフルパス 拡張子まで必要</param>
    /// <param name="content">= 書き込む内容</param>
    inline bool append_to_file(const string& path, const string& content) {
        ofstream f(path, ios::app);   // 追記はios::app
        if (!f) return false;

        f << content;
        f.close();
        return true;
    }

    /// <summary>
    /// jsonファイルを作成して追記保存する
    /// </summary>
    /// <param name="j">= jsonファイルへのフルパス 拡張子まで必要</param>
    /// <param name="content">= 書き込む内容</param>
    inline bool append_to_file(const string& path, const json& j) {
        ofstream f(path, ios::app);   // 追記はios::app
        if (!f) return false;

        f << j.dump(4);  // インデント
        f.close();
        return true;
    }

    /// <summary>
    /// jsonファイルを作成して追記保存する
    /// </summary>
    /// <param name="j">= jsonファイルへのフルパス 拡張子まで必要</param>
    /// <param name="content">= 書き込む内容</param>
    inline bool append_to_file(const string& path, const ordered_json& j) {
        ofstream f(path, ios::app);   // 追記はios::app
        if (!f) return false;

        f << j.dump(4);  // インデント
        f.close();
        return true;
    }

    /// <summary>
    /// 指定したフォルダ直下の指定した拡張子のファイルパスを全て取得する
    /// </summary>
    /// <param name="folder_path">= 検索をかけるファイルパス</param>
    /// <param name="extension">= 対象ファイルの拡張子</param>
    /// <returns>見つかったファイル全てのパス</returns>
    inline std::vector<string> get_all_file_path(const std::string& folder_path, const std::string& extension) {
        std::vector<std::string> paths;

        for (const auto& entry : fs::recursive_directory_iterator(folder_path)) {
            const fs::path& p = entry.path();

            if (fs::is_regular_file(p)) {
                const std::string file_ext = entry.path().extension().string();

                if (file_ext == "." + extension) {
                    paths.push_back(entry.path().string());
                }
            }
        }

        return paths;
    }
}

#endif
