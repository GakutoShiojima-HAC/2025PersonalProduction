#include "Engine/Core/LogSystem/SystemLog.h"
#include "Engine/Utils/Folder.h"
#include "Engine/Utils/SystemData.h"

// インデント
const string INDENT{ "    " };

void LogSystem::System::save(const std::string& folder_path, Data* data) {
    std::string content;

    MyLib::WINOSInfo winos_info = MyLib::get_winos_info();
    MyLib::CPUInfo cpu_info = MyLib::get_cpu_info();
    MyLib::GPUInfo gpu_info = MyLib::get_gpu_info();
    MyLib::MemoryInfo memory_info = MyLib::get_memory_info();
    MyLib::OpenGLInfo opengl_info = MyLib::get_opengl_info();

    content += "app start: " + data->start_time + "\n";
    content += "app end: " + data->end_time + "\n";
    content += "\n";
    content += "random seed: " + to_string(data->seed) + "\n";
    content += "\n";
    content += "OS:\n";
    content += INDENT + "info: " + winos_info.info + "\n";
    content += "\n";
    content += "CPU:\n";
    content += INDENT + "name: " + cpu_info.name + "\n";
    content += INDENT + "info: " + cpu_info.info + "\n";
    content += "\n";
    content += "GPU:\n";
    content += INDENT + "name: " + gpu_info.name + "\n";
    content += INDENT + "version: " + gpu_info.version + "\n";
    content += "\n";
    content += "memory:\n";
    content += INDENT + "total: " + to_string(memory_info.total) + "MB\n";
    content += INDENT + "max use: " + to_string(memory_info.maximum_usage) + "MB\n";
    content += "\n";
    content += "OpenGL:\n";
    content += INDENT + "use gpu: " + opengl_info.renderer + "\n";
    content += INDENT + "glsl version: " + opengl_info.glsl_version + "\n";

    MyLib::write_to_file(folder_path + "system.txt", content);
}
