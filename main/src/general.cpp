#include "general.h"

void outputString(const std::string outputLabel, const std::string output) {
    std::cout << "\033[1;33m" << outputLabel << "\033[0m" << output << "\n";
}

void outputError(const std::string errorString) {
    std::cerr << errorString << "\n";
    exit(1);
}
