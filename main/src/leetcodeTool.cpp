#include "leetcodeTool.h"

namespace fs = std::filesystem;

void launchIde(std::ifstream& config_file,
               fs::path& file_path) {
    char isActive =
        (char)std::stoi(config::extractConfig(
            config_file, publicConfigActiveIDE_string, true,
            publicConfigErrorNoIsActiveIde));
    if (!isActive)
        return;

    std::string ide_string = config::extractConfig(
        config_file, publicConfigChosenIDE_string, true,
        publicConfigErrorNoIdeString);
    outputString("Launch IDE Command : ",
                              ide_string);

    std::string command =
        ide_string + " \"" + file_path.string() + "\"";
    std::cout << std::flush;
    std::cerr << std::flush;

    system(command.c_str());
}

bool processCommands(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            std::string argument(argv[i]);
            argument.erase(0, 1); // get rid of the '-'
            int key = -1;

            /// Get command key, because you can't use
            /// switch cases with strings in cpp for some
            /// reason
            for (int j = 0; j < commandTags.size(); j++) {
                if (argument == commandTags[j]) {
                    key = j;
                    break;
                }
            }

            switch (key) {
            case -1:
                break;
            case HELP_COMMAND_INDX:
                std::cout << helpString << std::endl;
                break;
            case OPEN_LAST_INDX:
                /// TO-DO : add open last functionallity
                break;
            }

            if (key != -1)
                return true;
        }
    }

    return false;
}

void processArgCount(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << invalidInputError;
        exit(0);
    }

    if (processCommands(argc, argv)) {
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    processArgCount(argc, argv);

    config::ensureConfigExists(
        config::getConfigPath());
    std::ifstream config_file(config::getConfigPath());

    char copy_desc =
        (char)std::stoi(config::extractConfig(
            config_file, publicConfigCopyDesc_string, false,
            ""));
    char is_abs_directory =
        (char)std::stoi(config::extractConfig(
            config_file, publicConfigIsAbsolutPath_string,
            false, ""));
    languages chosen_language =
        getLanguageChar(config::extractConfig(
            config_file, publicConfigChosenLang_string,
            false, ""));

    std::string link(argv[1]);
    stringUtils::extractProblemFields(link);

    api::exportCurrentProblemInfo();

    fs::path created_file_path;
    std::ofstream code_file = config::createFileAndDir(
        problem_name, config_file, chosen_language,
        created_file_path, is_abs_directory);

    std::string clean_html_description =
        api::cleanHTML(content);

    if (copy_desc)
        stringUtils::exportProblemHeader(
            code_file, (int)chosen_language);

    std::string codeToken = "\"langSlug\":";
    codeToken.append("\"");
    codeToken.append(languageTokens[(char)chosen_language]);
    codeToken.append("\"");

    std::string problem_code = stringUtils::extractFromJson(
        problemDetail_copy, codeToken);
    stringUtils::exportCodeSnippet(
        problemDetail_copy, code_file, chosen_language);

    if (copy_desc)
        stringUtils::exportDescription(
            clean_html_description, code_file,
            (int)chosen_language);

    outputString(
        "Chosen Language : ",
        languageTokens[(char)chosen_language]);

    code_file.close();
    launchIde(config_file, created_file_path);

    return 0;
}
