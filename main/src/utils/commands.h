#pragma once

#include <iostream>
#include <filesystem>
#include "../../include/constants.h"
#include "config.h"

bool processCommands(int argc, char* argv[]);
void launchIde(std::ifstream& config_file,
               std::filesystem::path& file_path);
