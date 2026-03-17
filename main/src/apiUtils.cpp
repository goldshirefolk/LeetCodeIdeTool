#include "leetcodeTool.h"
using namespace LeetcodeToolConfig;

namespace apiUtils {
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
}