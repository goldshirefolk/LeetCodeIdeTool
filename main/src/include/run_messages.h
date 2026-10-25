#pragma once

#include <string>
#include "config_file.h"

inline const std::string helpString =
    R"(Run lct $LINK_TO_PROBLEM to copy over a problem and launch your ide.

-openlast or -ol to open up the last problem you created with lct

You can modify the behaviour of the tool by editing the config file found at ~/.lct)";

inline const std::string publicConfigErrorNoIsActiveIde =
    "whether or not the ide is supposed to be launched.";
inline const std::string publicConfigErrorNoIdeString =
    "the launch command for the ide." + publicConfigPath;
inline const std::string publicConfigErrorInvalidLanguage =
    "Invalid language found in config file! Please select a valid language." + publicConfigPath;
inline const std::string publicConfigErrorNoAbsolutePathDirectory =
    "No directory found in config for absolute path! " + publicConfigPath;

inline const std::string openningString = "\033[1;33mEnter problem link:\n\033[0m";
inline const std::string chooseLanguageString = "Enter your chosen programming language:\n";

inline const std::string image_warning =
    "\n!!! Problem has images, it is best you use the link !!!\n";

inline const std::string firstLaunchMessage =
    "Configure the tool from the public config file with your prefferences!\n";
inline const std::string noConfigFileMessage =
    "No config file found, created the default one at : ";
