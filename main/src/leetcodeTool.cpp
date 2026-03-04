#include "leetcodeTool.h"

using namespace LeetcodeToolConfig;
namespace fs = std::filesystem;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *response) {
    size_t total_size = size * nmemb;
    response->append((char *)contents, total_size);
    return total_size;
}

std::string makeGraphQLRequest(const std::string &query, const std::string &variables = "{}") {
    CURL *curl = curl_easy_init();
    std::string response;

    if (!curl)
        return "";

    std::string post_data = "{\"query\":\"" + query + "\",\"variables\":" + variables + "}";

    std::string escaped_query;
    for (char c : query) {
        if (c == '\n')
            escaped_query += "\\n";
        else if (c == '\"')
            escaped_query += "\\\"";
        else
            escaped_query += c;
    }

    post_data = "{\"query\":\"" + escaped_query + "\",\"variables\":" + variables + "}";

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");

    curl_easy_setopt(curl, CURLOPT_URL, "https://leetcode.com/graphql");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}

// Get problem list (50 problems at a time)
std::string getProblemList(int skip = 0, int limit = 50) {
    std::string query = getProblemsListQuery;

    std::string variables = R"({
        "categorySlug": "",
        "skip": )" + std::to_string(skip) +
                            R"(,
        "limit": )" + std::to_string(limit) +
                            R"(,
        "filters": {}
    })";

    return makeGraphQLRequest(query, variables);
}

// Get specific problem details by title
std::string getProblemDetail(const std::string &titleSlug) {
    std::string query = getProblemDetailsquery;

    std::string variables = R"({"titleSlug": ")" + titleSlug + "\"}";

    return makeGraphQLRequest(query, variables);
}

bool problem_has_images = false;

std::string problem_id;
std::string problem_title;
std::string problem_difficulty;

int desc_section_symbol_length = 32;

void outputString(const std::string outputLabel, const std::string output) {
    std::cout << "\033[1;33m" << outputLabel << "\033[0m" << output << "\n";
}

void outputError(const std::string errorString) {
    std::cerr << errorString << "\n";
    exit(1);
}

class stringExtractor {
private:
    static std::string extractToken(const std::string &string, const std::string &start_token, const std::string &end_token,
                                    int *newStartPos) {
        int start = string.find(start_token);

        if (start == std::string::npos) {
            exit(1);
        }
        start += start_token.length();

        int end = string.find(end_token, start);
        std::string finalString = string.substr(start, end - start);

        if (newStartPos)
            *newStartPos = end;
        return finalString;
    }

    static void detokenize(std::string &buffer) {
        /// Remove all the possible tokens we encounter
        for (int i = 0; i < genericDescTokens.size(); i++) {
            int token_pos_start = 0;
            int token_pos_end = 0;

            int token_length = (genericDescTokens[i]).length();

            while ((token_pos_end = buffer.find(genericDescTokens[i], token_pos_start)) != std::string::npos) {
                buffer.erase(token_pos_end, token_length);
                token_pos_start = token_pos_end - token_length;
            }
        }
    }

    static void processSection(std::string &buffer, int *section, std::ostream &outStream, int lang) {
        if (*section <= descSectionMax) {
            std::string token = descSectionTokens[*section];

            if (buffer.find(token) != std::string::npos) {
                std::string sectionSymbolBuffer;
                sectionSymbolBuffer.append(desc_section_symbol_length, descSectionSymbols[2]);

                outStream << sectionSymbolBuffer << '\n';
                (*section)++;
            }
        }

        /// Examples Section
        if (*section == 1) {
            if (buffer.find(descSectionTokens[0]) != std::string::npos) {
                buffer.insert(0, exampleSymbol_start);
                buffer.insert(buffer.length() - 1, exampleSymbol_end);
            }
        }
    }

    static void processNewLine(const std::string &description, int *start, int end, std::ostream &outStream,
                               const int maxNewLines) {
        int pos = end;
        int current = 0;
        while (pos < description.length() && description[pos] == newLine_token[0] && description[pos + 1] == newLine_token[1]) {
            if (current < maxNewLines)
                outStream << '\n';

            (pos) += 2;
            current++;
        }

        *start = pos;
    }

public:
    static int chosen_lang;

    static std::string nameFromLink(std::string &link) {
        return extractToken(link, "problems/", "/", NULL);
    }

    static std::string extractFromJson(std::string &json, const std::string &tag) {
        int newPos = 0;
        std::string json_extracted = extractToken(json, tag, ",\"", &newPos);

        json.erase(0, newPos);
        return json_extracted;
    }

    typedef enum {
        SECTION_START = 2,
        SECTION_EXAMPLES = 1,
        SECTION_CONSTRAINTS = 1
    } descSectionMaxNewLines;

    static void exportProblemHeader(std::ostream &outStream, int lang) {
        outStream << commentDelims[lang][COMMENT_DELIMS_START_INDX] << "\n";

        std::string start_buffer;

        start_buffer += problem_id + ". " + problem_title + " [" + problem_difficulty + "]";
        start_buffer = std::regex_replace(start_buffer, std::regex("\""), "");

        outStream << start_buffer << '\n';

        /// Show link
        start_buffer.clear();

        start_buffer = generatedLinkStart;
        start_buffer += problem_title;
        start_buffer = std::regex_replace(start_buffer, std::regex(" "), "-");
        start_buffer = std::regex_replace(start_buffer, std::regex("\""), "");
        std::transform(start_buffer.begin(), start_buffer.end(), start_buffer.begin(), ::tolower);
        start_buffer += '/';
        outStream << start_buffer << '\n';

        if (problem_has_images) {
            outStream << image_warning;
        }

        outStream << commentDelims[lang][COMMENT_DELIMS_END_INDX] << "\n\n\n";
    }

    static void exportDescription(std::string &description, std::ostream &outStream, int lang) {
        int start = 0;
        int end = 0;

        bool first_buffer = true;

        int section = 0;

        outStream << commentDelims[lang][COMMENT_DELIMS_START_INDX] << "\n";

        while ((end = description.find(newLine_token, start)) != std::string::npos) {
            std::string buffer = description.substr(start, end - start);

            detokenize(buffer);

            if (first_buffer) {
                buffer.erase(0, 1);
                first_buffer = false;
            }

            if (section == 0) {
                int buffer_length = buffer.length();

                while (buffer_length > descLineMaxLength) {
                    int erasePos = descLineMaxLength;
                    while (buffer[--erasePos] != ' ')
                        ;

                    outStream << buffer.substr(0, erasePos) << '\n';
                    buffer.erase(0, erasePos);
                    if (buffer[0] == ' ')
                        buffer.erase(0, 1);

                    buffer_length -= erasePos;

                    desc_section_symbol_length = erasePos;
                }

                if (buffer_length > desc_section_symbol_length) {
                    desc_section_symbol_length = buffer_length;
                }
            } else if (section == 1) {
                buffer = std::regex_replace(buffer, std::regex("\\\\"), "");
            } else if (section == 2) {  // Constraints
                buffer.insert(0, constraintsLineSymbol_start);
            }

            processSection(buffer, &section, outStream, lang);

            outStream << buffer;

            processNewLine(description, &start, end, outStream, maxSectionNewLines[section]);

            buffer.clear();
        }

        outStream << commentDelims[lang][COMMENT_DELIMS_END_INDX] << "\n\n\n";
    }

    static void prepareCodeSnippet(std::string &description) {
        int start = description.find("code\":", 0);
        int skipLength = sizeof("codep:");
        description = description.substr(start + skipLength, description.size() - skipLength);

        int code_snippet_end = description.find("\"},{\"lang\"");
        description = description.substr(0, code_snippet_end);
    }

    static void exportCodeSnippet(std::string &description, std::ostream &outStream, languages chosen_language) {
        prepareCodeSnippet(description);

        for (int i = 0; i < description.size(); i++) {
            if (i < description.size() - 1) {
                if (description[i] == '\\' && description[i + 1] == 'n') {
                    outStream << '\n';
                    i += 2;
                }

                if (i > description.size()) {
                    break;
                }

                if (chosen_language == LANG_PYTHON || chosen_language == LANG_PYTHON3) {
                    if (description[i] == '\\')
                        i++;
                }

                if (i > description.size()) {
                    break;
                }
            }
            outStream << description[i];
        }

        outStream << "\n\n\n";
    }
};

std::string cleanHTML(const std::string &html) {
    std::string result = html;

    if (result.find("<img") != std::string::npos) {
        problem_has_images = true;
    }

    result = std::regex_replace(result, std::regex("<sup>"), " to the power of : ");

    /// Remove HTML tags
    std::regex tagRegex("<[^>]*>");
    result = std::regex_replace(result, tagRegex, "");

    /// Decode HTML entities
    result = std::regex_replace(result, std::regex("&amp;"), "&");
    result = std::regex_replace(result, std::regex("&lt;"), "<");
    result = std::regex_replace(result, std::regex("&gt;"), ">");
    result = std::regex_replace(result, std::regex("&quot;"), "\"");
    result = std::regex_replace(result, std::regex("&#39;"), "'");
    result = std::regex_replace(result, std::regex("&nbsp;"), " ");

    return result;
}

languages getLanguageChar(const std::string &lang) {
    for (char i = 0; i < LANGUAGE_COUNT; i++) {
        if (lang == languageTokens[i]) {
            return (languages)i;
        }
    }

    std::cout << "INVALID LANGUAGE FOUND IN CONFIG FILE, PLEASE CHECK THE CONFIG FILE @ " << "\nQuitting...\n\n";
    exit(1);
    return LANG_INVALID;
}

std::string extractConfig(std::ifstream &public_config_file, const std::string configString, bool mandatory, std::string errorMessage) {
    // reset file
    public_config_file.clear();
    public_config_file.seekg(0, std::ios::beg);

    std::string buffer;
    do {
        std::getline(public_config_file, buffer);

        if (buffer.find(publicConfigConfigEndTag_string) != std::string::npos) {
            if (mandatory) {
                outputError(errorMessage);
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

void launchIde(std::ifstream& config_file, fs::path &file_path) {
    char isActive = (char)std::stoi(extractConfig(config_file, publicConfigActiveIDE_string, true, publicConfigErrorNoIsActiveIde));
    if (!isActive)
        return;

    std::string ide_string = extractConfig(config_file, publicConfigChosenIDE_string, true, publicConfigErrorNoIdeString);
    outputString("Launch IDE Command : ", ide_string);

    std::string command = ide_string + " " + file_path.string();
    system(command.c_str());
}

fs::path createDir(const std::string &problem_name, const char is_abs_path) {
    std::error_code ec;

    fs::path base;

    if (!is_abs_path) {
        base = fs::current_path();
    } else {
        const char *home = std::getenv("HOME");
        base = std::string(home) + absolutPathFolder;
    }

    fs::path current_new_dir = fs::path(problem_name);

    fs::path dir = base / current_new_dir;

    if (fs::create_directories(dir, ec)) {
    } else if (ec) {
        std::cerr << "Error: " << ec.message() << '\n';
    } else {
    }

    return dir;
}

void unescapeNewlines(std::string& input) {
    size_t p = 0;
    while ((p = input.find("\\n", p)) != std::string::npos) {
        input.replace(p, 2, "\n");
        p++;
    }
}

std::ofstream createFileAndDir(std::string &problem_name, std::ifstream& config_file, 
    languages chosen_language, fs::path &file_path, const char is_abs_path) {

    file_path = createDir(problem_name, is_abs_path);

    std::string code_file_name = problem_name;
    code_file_name.append(codeFileSufixes[chosen_language]);
    file_path /= code_file_name;

    outputString(createdFileString, file_path.string());

    std::ofstream code_file_path(file_path);

    std::string custom_header_config_tag = chosen_language + "_header";

    std::string header;
    std::string custom_header = extractConfig(config_file, custom_header_config_tag, false, ""); 

    if (custom_header != "") {
        header = custom_header;
        unescapeNewlines(header);
    } else {
        header = codeSnippetPrefixes[(int)chosen_language];
    }

    if ((int)chosen_language < codeSnippetPrefixes.size()) {
        code_file_path << header << codeSnippetPrefixNewlines;
    }

    return code_file_path;
}

void exportCurrentProblemInfo() {
    problem_id = std::regex_replace(problem_id, std::regex("\""), "");
    problem_title = std::regex_replace(problem_title, std::regex("\""), "");
    problem_difficulty = std::regex_replace(problem_difficulty, std::regex("\""), "");

    std::string diff_color_wrapper_start = "\033[1;";
    std::string diff_color_wrapper_mid = "m";
    std::string diff_color_wrapper_end = "\033[0m";

    std::string diff_color_wrapper_mid_add;
    if (problem_difficulty == "Easy") {
        diff_color_wrapper_mid_add = "32";
    } else if (problem_difficulty == "Medium") {
        diff_color_wrapper_mid_add = "33";
    } else {
        diff_color_wrapper_mid_add = "31";
    }

    std::cout << "[" << problem_id << ". " << problem_title << " (" << diff_color_wrapper_start + diff_color_wrapper_mid_add + diff_color_wrapper_mid + problem_difficulty + diff_color_wrapper_end
              << ")]\n\n";
}

inline std::string getConfigPath() {
    const char* home = std::getenv("HOME");
    if (home == nullptr) {
        return configPath; 
    }
    return std::string(home) + configPath;
}

inline const std::string publicConfigFilePath = getConfigPath();

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
        std::cout << "No config file found, created the default one at : " << path << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Please enter a problem link as the second argument!\n";
        return 0;
    }

    char copy_desc = 1;
    char is_abs_directory = 1;

    ensureConfigExists(getConfigPath());
    std::ifstream config_file(publicConfigFilePath);
    // if (!config_file) {
    //     std::ofstream new_config_file(publicConfigFileName);
    //     new_config_file << defaultConfig;

    //     config_file.close();
    //     config_file.clear();
    //     config_file.open(publicConfigFileName);

    //     std::cout << "No config file could be found. The default one was created and is being used.";
    // }

    copy_desc = (char)std::stoi(extractConfig(config_file, publicConfigCopyDesc_string, false, ""));
    is_abs_directory = (char)std::stoi(extractConfig(config_file, publicConfigIsAbsolutPath_string, false, ""));
    languages chosen_language = getLanguageChar(extractConfig(config_file, publicConfigChosenLang_string, false, ""));

    std::string link(argv[1]);

    std::string problem_name = stringExtractor::nameFromLink(link);
    std::string problem_detail = getProblemDetail(problem_name);
    std::string problemDetail_copy = problem_detail;

    problem_id = stringExtractor::extractFromJson(problem_detail, "\"questionId\":");

    problem_title = stringExtractor::extractFromJson(problem_detail, "\"title\":");

    std::string content = stringExtractor::extractFromJson(problem_detail, "\"content\":");

    problem_difficulty = stringExtractor::extractFromJson(problem_detail, "\"difficulty\":");

    exportCurrentProblemInfo();

    fs::path created_file_path;
    std::ofstream code_file = createFileAndDir(problem_name, config_file, chosen_language, created_file_path, is_abs_directory);

    std::string clean_html_description = cleanHTML(content);

    if (copy_desc)
        stringExtractor::exportProblemHeader(code_file, (int)chosen_language);

    std::string codeToken = "\"langSlug\":";
    codeToken.append("\"");
    codeToken.append(languageTokens[(char)chosen_language]);
    codeToken.append("\"");

    std::string problem_code = stringExtractor::extractFromJson(problemDetail_copy, codeToken);

    stringExtractor::exportCodeSnippet(problemDetail_copy, code_file, chosen_language);


    if (copy_desc)
        stringExtractor::exportDescription(clean_html_description, code_file, (int)chosen_language);

    outputString("Chosen Language : ", languageTokens[(char)chosen_language]);        

    launchIde(config_file, created_file_path);

    return 0;
}