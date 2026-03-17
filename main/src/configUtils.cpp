#include "leetcodeTool.h"
using namespace LeetcodeToolConfig;
#include "stringUtils.cpp"
namespace fs = std::filesystem;

namespace configUtils {
    void ensureConfigExists(const std::string& path) {
        namespace fs = std::filesystem;
        
        fs::path configPath(path);
        if (!fs::exists(configPath.parent_path())) {
            fs::create_directories(configPath.parent_path());
        }

        if (!fs::exists(configPath)) {
            std::ofstream outFile(path);
            outFile << defaultConfig;
            outFile.close();

            std::cout << noConfigFileMessage << path << std::endl;
        }
    }

    std::string extractConfig(std::ifstream &public_config_file, const std::string configString, bool mandatory, std::string errorMessage) {
        /// Reset file
        public_config_file.clear();
        public_config_file.seekg(0, std::ios::beg);

        std::string buffer;
        do {
            std::getline(public_config_file, buffer);

            if (buffer.find(publicConfigConfigEndTag_string) != std::string::npos) {
                if (mandatory) {
                    stringUtils::outputError(errorMessage);
                }

                return "";
            }

            if (configString == publicConfigPrevLaunched_string && buffer == publicConfigConfigEndTag_string) {
                return buffer;
            }
        } while (buffer.find(configString) == std::string::npos || buffer.find("=") == std::string::npos);

        int p = 0;
        while (buffer[p] != '=') {
            p++;
        }

        p++;

        while (buffer[p] == ' ') {
            p++;
        }

        buffer.erase(0, p);

        return buffer;
    }

    fs::path createDir(const std::string &problem_name, const char is_abs_path, const std::string &abs_path_name) {
        std::error_code ec;

        fs::path base;

        if (!is_abs_path) {
            base = fs::current_path();
        } else {
            const char *home = std::getenv("HOME");
            base = std::string(home) + abs_path_name;
        }

        fs::path current_new_dir = fs::path(problem_name);
        fs::path dir = base / current_new_dir;

        if (fs::create_directories(dir, ec)) {
        } else if (ec) {
            std::cerr << "Error: " << ec.message() << '\n';
        }

        return dir;
    }

    std::ofstream createFileAndDir(std::string &problem_name, std::ifstream& config_file, 
        languages chosen_language, fs::path &file_path, const char is_abs_path) {


        std::string absolute_directory_path = extractConfig(config_file, publicConfigAbsolutePathDir_string, true, "");
        if (absolute_directory_path[0] != '/') {
            absolute_directory_path = '/' + absolute_directory_path;
        }
        if (absolute_directory_path[absolute_directory_path.length() - 1] != '/') {
            absolute_directory_path += '/';
        }
        file_path = createDir(problem_name, is_abs_path, absolute_directory_path);

        std::string code_file_name = problem_name;
        code_file_name.append(codeFileSufixes[chosen_language]);
        file_path /= code_file_name;

        stringUtils::outputString(createdFileString, file_path.string());

        std::ofstream code_file_path(file_path);

        std::string custom_header_config_tag = chosen_language + "_header";

        std::string header;
        std::string custom_header = extractConfig(config_file, custom_header_config_tag, false, ""); 

        if (custom_header != "") {
            header = custom_header;
            stringUtils::unescapeNewlines(header);
        } else {
            header = codeSnippetPrefixes[(int)chosen_language];
        }

        if ((int)chosen_language < codeSnippetPrefixes.size()) {
            code_file_path << header << codeSnippetPrefixNewlines;
        }

        return code_file_path;
    }


    inline std::string getConfigPath() {
        const char* home = std::getenv("HOME");
        if (home == nullptr) {
            return configPath; 
        }
        return std::string(home) + configPath;
    }
}
