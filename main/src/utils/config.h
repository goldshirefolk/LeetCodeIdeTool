#pragma once

#include "stringUtils.h"
#include <filesystem>
#include <fstream>
#include <iostream>
using namespace LeetcodeToolConfig;

namespace config {
void ensureConfigExists(const std::string& path);
std::string extractConfig(std::ifstream& public_config_file,
                          const std::string configString, bool mandatory,
                          std::string errorMessage);
std::filesystem::path createDir(const std::string& problem_name,
                                const char is_abs_path,
                                const std::string& abs_path_name);
std::ofstream createFileAndDir(std::string& problem_name,
                               std::ifstream& config_file,
                               languages chosen_language,
                               std::filesystem::path& file_path,
                               const char is_abs_path);

inline std::string getConfigPath() {
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        return configPath;
    }
    return std::stringUtils.home) + configPath;
}

} // namespace config
