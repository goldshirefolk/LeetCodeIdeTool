#include "leetcodeTool.h"
using namespace LeetcodeToolConfig;

namespace stringUtils {
    void outputString(const std::string outputLabel, const std::string output) {
        std::cout << "\033[1;33m" << outputLabel << "\033[0m" << output << "\n";
    }

    void outputError(const std::string errorString) {
        std::cerr << errorString << "\n";
        exit(1);
    }

    std::string extractToken(const std::string &string, const std::string &start_token, const std::string &end_token,
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

    void detokenize(std::string &buffer) {
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

    void processSection(std::string &buffer, int *section, std::ostream &outStream, int lang) {
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

    void processNewLine(const std::string &description, int *start, int end, std::ostream &outStream,
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

     int chosen_lang;

     std::string nameFromLink(std::string &link) {
        return extractToken(link, "problems/", "/", NULL);
    }

     std::string extractFromJson(std::string &json, const std::string &tag) {
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

    void exportProblemHeader(std::ostream &outStream, int lang) {
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

    void exportDescription(std::string &description, std::ostream &outStream, int lang) {
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

    void prepareCodeSnippet(std::string &description) {
        int start = description.find("code\":", 0);
        int skipLength = sizeof("codep:");
        description = description.substr(start + skipLength, description.size() - skipLength);

        int code_snippet_end = description.find("\"},{\"lang\"");
        description = description.substr(0, code_snippet_end);
    }

    void exportCodeSnippet(std::string &description, std::ostream &outStream, languages chosen_language) {
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

    void unescapeNewlines(std::string& input) {
        size_t p = 0;
        while ((p = input.find("\\n", p)) != std::string::npos) {
            input.replace(p, 2, "\n");
            p++;
        }
    }
}