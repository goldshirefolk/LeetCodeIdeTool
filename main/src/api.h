#pragma once

#include "include/constants.h"

#include <curl/curl.h>
#include <iostream>
#include <json/json.h>
#include <regex>

namespace api {
size_t WriteCallback(void* contents, size_t size, size_t nmemb,
                     std::string* response);
std::string makeGraphQLRequest(const std::string& query,
                               const std::string& variables);
std::string cleanHTML(const std::string& html);
std::string getProblemDetail(const std::string& titleSlug);
void exportCurrentProblemInfo();
} // namespace api
