#pragma once

#include "include/constants.h"
#include "config.h"

#include <iostream>
#include <filesystem>

bool processCommands(int argc, char* argv[]);

void launchIde(std::ifstream& config_file,
               std::filesystem::path& file_path);
