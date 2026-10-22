#pragma once

#include "../../include/config.h"
#include <curl/curl.h>
#include <iostream>
#include <json/json.h>
#include <regex>

using namespace LeetcodeToolConfig;

namespace apiUtils {
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response);
std::string makeGraphQLRequest(const std::string& query, const std::string& variables);
std::string cleanHTML(const std::string& html);
std::string getProblemDetail(const std::string& titleSlug);
void exportCurrentProblemInfo();
} // namespace apiUtils
