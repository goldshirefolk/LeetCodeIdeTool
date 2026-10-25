#include "leetcode_tool.h"

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
