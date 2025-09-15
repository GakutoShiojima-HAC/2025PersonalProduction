#include "Engine/Core/LogSystem/LogManager.h"
#include "Engine/Utils/MyTime.h"
#include "Engine/Utils/Folder.h"
#include "Engine/Core/LogSystem/SystemLog.h"

LogManager& LogManager::get_instance() {
    // static変数のインスタンスは１つ
    // インスタンス化も１回のみ
    static LogManager self;
    return self;
}

void LogManager::app_start() {
    start_time = MyLib::get_time_info().time_info_to_string();
}

void LogManager::save(const std::string& folder_path) {
    // 現在時刻
    const std::string current_time = MyLib::get_time_info().time_info_to_string();

    // ログフォルダを作成
    MyLib::create_folder(folder_path);

    // システムログ
    {
        LogSystem::System::Data data;
        data.start_time = start_time;
        data.end_time = current_time;
        data.seed = random_seed_;

        LogSystem::System::save(folder_path, &data);
    }

}

void LogManager::set_seed(unsigned int seed) {
    random_seed_ = seed;
}
