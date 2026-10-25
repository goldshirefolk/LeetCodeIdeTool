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

inline const std::string absolutPathFolder = "/leetCodeProblems/";
inline const std::string createdFileString = "Created File : ";
