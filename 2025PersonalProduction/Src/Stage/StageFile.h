#ifndef STAGE_FILE_H_
#define STAGE_FILE_H_

#include <string>
#include <vector>

class StageFile {
public:
    StageFile();

    ~StageFile() = default;

public:
    std::string get_path(int stage_id) const;

private:
    std::string tutorial_{ "" };

    std::vector<std::string> stages_{ "" };

public:
    // ÉRÉsÅ[ã÷é~
    StageFile(const StageFile& other) = delete;
    StageFile& operator = (const StageFile& other) = delete;
};

#endif
