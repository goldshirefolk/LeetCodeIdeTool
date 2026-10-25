#pragma once

#include <string>

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
