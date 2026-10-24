#include "languages.h"

languages getLanguageChar(const std::string& lang) {
    for (char i = 0; i < LANGUAGE_COUNT; i++) {
        if (lang == languageTokens[i]) {
            return (languages)i;
        }
    }

    outputError(
        publicConfigErrorInvalidLanguage);
    return LANG_INVALID;
}
