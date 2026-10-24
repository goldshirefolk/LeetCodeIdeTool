#pragma once

#include <iostream>
#include <regex>
#include <filesystem>
#include <fstream>

#include "../../include/constants.h"
#include "languages.h"
#include "api.h"

namespace stringUtils {
std::string extractToken(const std::string& string,
                         const std::string& start_token,
                         const std::string& end_token, int* newStartPos);
void detokenize(std::string& buffer);

void processSection(std::string& buffer, int* section, std::ostream& outStream,
                    int lang);
void processNewLine(const std::string& description, int* start, int end,
                    std::ostream& outStream, const int maxNewLines);

std::string nameFromLink(std::string& link);
std::string extractFromJson(std::string& json, const std::string& tag);

void exportProblemHeader(std::ostream& outStream, int lang);
void exportDescription(std::string& description, std::ostream& outStream,
                       int lang);

void prepareCodeSnippet(std::string& description);
void exportCodeSnippet(std::string& description, std::ostream& outStream,
                       languages chosen_language);
void unescapeNewlines(std::string& input);
void extractProblemFields(std::string& link);
} // namespace stringUtils
