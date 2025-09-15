// -----------------------------------------------------------------------------------------
//  File        : SystemData.h
//  Author      : Shiojima Gakuto
//  Created     : 2025/09/15
//  Updated     : 2025/09/15
//  Description : システム関係の情報取得関数群
//
//  注意：本ソースコードの無断転載・コードのコピー・貼り付けによる流用・再配布を禁止します。
// -----------------------------------------------------------------------------------------

#ifndef MYLIB_SYSTEM_DATA_H_
#define MYLIB_SYSTEM_DATA_H_

#include <string>
#include <GL/glut.h>	// GPU
#include <windows.h>	// CPU, WINOS
#include <psapi.h>		// MEMORY

using namespace std;

namespace MyLib {
#pragma region [GPU]
    /// <summary>
    /// GPU情報構造体
    /// </summary>
    struct GPUInfo {
        string name;
        string version;
    };

    /// <summary>
    /// PCのGPU情報を取得
    /// </summary>
    inline GPUInfo get_gpu_info() {
        GPUInfo result;

        result.name = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        result.version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        return result;
    }
#pragma endregion

#pragma region [CPU]
    /// <summary>
    /// CPU情報構造体
    /// </summary>
    struct CPUInfo {
        string name;
        string info;
    };

    /// <summary>
    /// PCのCPU情報を取得
    /// </summary>
    inline CPUInfo get_cpu_info() {
        CPUInfo result;

        // CPUの名前を取得するラムダ式
        auto get_name = []() {
            char cpu_name[256];
            DWORD size = sizeof(cpu_name);
            DWORD type = 0;

            // レジストリキー(CPU情報のパス)を開く
            HKEY h_key;
            const char* path = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";
            if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &h_key) != ERROR_SUCCESS)
                return string{ "Unknown CPU" };


            // "ProcessorNameString" という値をレジストリから取得
            if (RegQueryValueExA(h_key, "ProcessorNameString", nullptr, &type, reinterpret_cast<LPBYTE>(cpu_name), &size) != ERROR_SUCCESS) {
                RegCloseKey(h_key);
                return  string{ "Unknown CPU" };
            }

            // レジストリキーを閉じる
            RegCloseKey(h_key);
            return string(cpu_name);
            };

        // CPUの情報を取得するラムダ式
        auto get_info = []() {
            SYSTEM_INFO sys_info;
            GetSystemInfo(&sys_info);

            // CPU アーキテクチャの確認
            string arch;
            switch (sys_info.wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_AMD64: arch = "x64"; break;
            case PROCESSOR_ARCHITECTURE_INTEL: arch = "x86"; break;
            case PROCESSOR_ARCHITECTURE_ARM: arch = "ARM"; break;
            default: arch = "Unknown"; break;
            }

            return "Architecture: " + arch + ", Core: " + std::to_string(sys_info.dwNumberOfProcessors);
            };

        result.name = get_name();
        result.info = get_info();

        return result;
    }
#pragma endregion

#pragma region [MEMORY]
    /// <summary>
    /// メモリ情報構造体
    /// </summary>
    struct MemoryInfo {
        int total;
        int current_usege;
        int maximum_usage;
    };

    /// <summary>
    /// PCのメモリ情報を取得
    /// </summary>
    inline MemoryInfo get_memory_info() {
        MemoryInfo result;

        // 総メモリ量を取得するラムダ式
        auto get_total = []() {
            MEMORYSTATUSEX mem_status;
            mem_status.dwLength = sizeof(MEMORYSTATUSEX);
            GlobalMemoryStatusEx(&mem_status);
            return (int)(mem_status.ullTotalPhys / (1024 * 1024));
            };

        // 現在のメモリ使用量を取得するラムダ式
        auto get_current = []() {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
                return (int)(pmc.WorkingSetSize / (1024 * 1024));
            }
            return -1; // 取得できない場合
            };

        // このアプリケーションの最大メモリ使用量を取得するラムダ式
        auto get_maximum = []() {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
                return (int)(pmc.PeakWorkingSetSize / (1024 * 1024));
            }
            return -1; // 取得できない場合
            };

        result.total = get_total();
        result.current_usege = get_current();
        result.maximum_usage = get_maximum();

        return result;
    }
#pragma endregion

#pragma region [WINOS]
    /// <summary>
    /// PCのOS情報構造体
    /// </summary>
    struct WINOSInfo {
        string info;
        string name;
        string build_id;
        string edition;
    };

    /// <summary>
    /// PCのOS情報を取得
    /// </summary>
    inline WINOSInfo get_winos_info() {
        WINOSInfo result;

        HKEY h_key;
        const char* path = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, path, 0, KEY_READ, &h_key) != ERROR_SUCCESS) {
            // レジストリが開けなかった場合は空文字列のまま返す
            return result;
        }

        auto query_string_value = [&](const char* valueName) -> string {
            char buffer[256];
            DWORD size = sizeof(buffer);
            DWORD type = 0;
            if (RegQueryValueExA(h_key, valueName, nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &size) == ERROR_SUCCESS) {
                if (type == REG_SZ || type == REG_EXPAND_SZ) {
                    return string(buffer, size - 1); // sizeはバイト数。-1で終端文字削除
                }
            }
            return "";
            };

        result.name = query_string_value("ProductName");
        result.build_id = query_string_value("CurrentBuildNumber");
        result.edition = query_string_value("EditionID");

        // レジストリキーを閉じる
        RegCloseKey(h_key);

        // Windows11なら書き換える（Build 22000以上）
        std::string version_name = "Windows 10";
        if (!result.build_id.empty()) {
            try {
                int build = stoi(result.build_id);
                if (build >= 22000) {
                    version_name = "Windows 11";
                    result.name = "Windows 11 " + result.edition;
                }
            }
            catch (...) {}
        }

        // エディション名変換
        string edition = result.edition;
        if (edition == "Core") edition = "Home";
        else if (edition == "Professional") edition = "Pro";

        // ビルド番号から簡易的なマーケティング版の表記（23H2など）を生成
        string marketing_version = "";
        try {
            int build = stoi(result.build_id);
            if (build >= 22631) marketing_version = "23H2";
            else if (build >= 22621) marketing_version = "22H2";
            else if (build >= 22000) marketing_version = "21H2";
            else if (build >= 19044) marketing_version = "21H2";
            else if (build >= 19043) marketing_version = "21H1";
            else if (build >= 19042) marketing_version = "20H2";
            else if (build >= 19041) marketing_version = "2004";
        }
        catch (...) {}

        // info 文字列を構築
        result.info = version_name + " " + edition;
        if (!marketing_version.empty()) {
            result.info += " " + marketing_version;
        }
        if (!result.build_id.empty()) {
            result.info += " Build " + result.build_id;
        }

        return result;
    }

    /*
        エディションの種類
        Core: Windows Home
        Professional: Windows Pro
        Enterprise: 法人向け
        Education: 教育機関向け
    */
#pragma endregion

#pragma region [OpenGL]
    /// <summary>
    /// 使用しているOpenGL情報構造体
    /// </summary>
    struct OpenGLInfo {
        std::string renderer;
        std::string version;
        std::string glsl_version;
    };

    /// <summary>
    /// 使用しているOpenGL情報を取得
    /// </summary>
    inline OpenGLInfo get_opengl_info() {
        const GLubyte* renderer_cstr = glGetString(GL_RENDERER);    // GPU名
        const GLubyte* version_cstr = glGetString(GL_VERSION);      // OpenGLバージョン
        const GLubyte* glsl_version_cstr = glGetString(GL_SHADING_LANGUAGE_VERSION);    // GLSLバージョン

        OpenGLInfo result;
        result.renderer = renderer_cstr ? reinterpret_cast<const char*>(renderer_cstr) : "Unknown";
        result.version = version_cstr ? reinterpret_cast<const char*>(version_cstr) : "Unknown";
        result.glsl_version = glsl_version_cstr ? reinterpret_cast<const char*>(glsl_version_cstr) : "Unknown";

        return result;
    }
#pragma endregion

}

#endif
