#pragma once

#include <string>
#include <vector>

#define COMMENT_DELIMS_START_INDX 0
#define COMMENT_DELIMS_END_INDX 1

#define IDE_CONFIG_VAL_VSCODE 0
#define IDE_CONFIG_VAL_NVIM 1

inline bool problem_has_images = false;
inline std::string problem_name;
inline std::string problem_detail;
inline std::string problemDetail_copy;
inline std::string problem_id;
inline std::string problem_title;
inline std::string problem_difficulty;
inline std::string content;
inline int desc_section_symbol_length = 32;

//==GENERAL==========================================================================================================================================

inline const std::string configPath = "/.lct/publicConfig.txt";
inline const std::string defaultConfig = R"((configStart)

copy_desc = 1 // whether or not the description should be copied into the file
path = 1 // 1 for absolute path, 0 for making the directory in the current path
launch_ide = 1
ide_command = code
absolute_path_dir = leetcodeProblems/

chosen_lang = cpp
[   
    "cpp",         //  cpp
    "c",           //  c
    "csharp",      //  csharp
    "java",        //  java
    "python3",     //  python3
    "python",      //  python
    "javascript",  //  javascript
    "typescript",  //  typescript
    "golang",      //  go
    "kotlin",      //  kotlin
    "swift",       //  swift
    "php",         //  php
    "dart",        //  dart
    "scala",       //  scala
    "elixir",      //  elixir
    "erlang",      //  erlang
    "racket",      //  racket
    "rust",        //  rust
]

// If you want to use a custom header for a specific language, declare a field like so
// "LANG_header" and type in the header and use '\n' to separate the lines

(configEnd))";

inline const std::string helpString =
    R"(Run lct $LINK_TO_PROBLEM to copy over a problem and launch your ide.

-openlast or -ol to open up the last problem you created with lct

You can modify the behaviour of the tool by editing the config file found at ~/.lct)";

inline const std::string publicConfigFileName = "publicConfig.txt";
inline const std::string firstLaunchMessage =
    "Configure the tool from the public config file with your prefferences!\n";
inline const std::string noConfigFileMessage =
    "No config file found, created the default one at : ";
inline const std::string publicConfigActiveIDE_string = "launch_ide";
inline const std::string publicConfigChosenIDE_string = "ide_command";
inline const std::string publicConfigCopyDesc_string = "copy_desc";
inline const std::string publicConfigAbsolutePathDir_string = "absolute_path_dir";
inline const std::string publicConfigChosenLang_string = "chosen_lang";
inline const std::string publicConfigPrevLaunched_string = "prev_launched";
inline const std::string publicConfigIsAbsolutPath_string = "path";
inline const std::string publicConfigConfigEndTag_string = "(configEnd)";

inline const std::string publicConfigPath = "(~/.lct/publicConfig.txt)";

inline const std::string publicConfigErrorNoIsActiveIde =
    "whether or not the ide is supposed to be launched.";
inline const std::string publicConfigErrorNoIdeString =
    "the launch command for the ide." + publicConfigPath;
inline const std::string publicConfigErrorInvalidLanguage =
    "Invalid language found in config file! Please select a valid language." + publicConfigPath;
inline const std::string publicConfigErrorNoAbsolutePathDirectory =
    "No directory found in config for absolute path! " + publicConfigPath;

inline const std::string openningString = "\033[1;33mEnter problem link:\n\033[0m";
inline const std::string chooseLanguageString = "Enter your chosen programming language:\n";

inline const std::string image_warning =
    "\n!!! Problem has images, it is best you use the link !!!\n";

inline const std::string newLine_token = "\\n";

inline const std::vector<std::string> genericDescTokens = {"\\u200b", "\\t"};
inline const std::vector<std::string> descSectionTokens = {"Example", "Constraints"};

inline const std::string exampleSymbol_start = "(";
inline const std::string exampleSymbol_end = ")";

inline const std::string constraintsLineSymbol_start = "(*) ";

inline const std::vector<std::string> descSectionSymbols_start = {"/*"};
inline const std::vector<std::string> descSectionSymbols_end = {"*/"};
inline const std::vector<char> descSectionSymbols = {'=', '#', '-'};
inline const std::vector<int> maxSectionNewLines = {2, 2, 1};

inline const int descLineMaxLength = 96;
inline const int descSectionMax = 1;

inline const std::string invalidInputError =
    "Please enter a problem link as the second argument!\nUse \"lct --help\" to "
    "see the possible commands.";

inline const std::vector<std::string> commandTags = {"help", "l"};
#define HELP_COMMAND_INDX 0
#define OPEN_LAST_INDX 1

//==QUERIES=========================================================================================================================================
inline const std::string generatedLinkStart = "https://leetcode.com/problems/";

inline const std::string getProblemDetailsquery = R"(
        query questionData($titleSlug: String!) {
            question(titleSlug: $titleSlug) {
                questionId
                questionFrontendId
                title
                titleSlug
                content
                difficulty
                exampleTestcases
                codeSnippets {
                    lang
                    langSlug
                    code
                }
            }
        }
    )";

inline const std::string getProblemsListQuery = R"(
        query problemsetQuestionList($categorySlug: String, $limit: Int, $skip: Int, $filters: QuestionListFilterInput) {
            problemsetQuestionList: questionList(
                categorySlug: $categorySlug
                limit: $limit
                skip: $skip
                filters: $filters
            ) {
                total: totalNum
                questions: data {
                    acRate
                    difficulty
                    frontendQuestionId: questionFrontendId
                    paidOnly: isPaidOnly
                    title
                    titleSlug
                    topicTags {
                        name
                    }
                }
            }
        }
    )";

inline const std::string absolutPathFolder = "/leetCodeProblems/";
inline const std::string createdFileString = "Created File : ";
