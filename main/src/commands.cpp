#include "commands.h"

int getArgumentKey(const std::string &argument) {
    for (int j = 0; j < commandTags.size(); j++) {
        if (argument == commandTags[j]) {
            return j;
        }
    }

    return -1;
}

bool processCommands(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            std::string argument(argv[i]);
            argument.erase(0, 1); // get rid of the '-'
            int key = getArgumentKey(argument);

            switch (key) {
            case -1:
                break;
            case HELP_COMMAND_INDX:
                std::cout << helpString << std::endl;
                break;
            case OPEN_LAST_INDX:
                /// TO-DO : add open last functionallity
                break;
            }

            if (key != -1)
                return true;
        }
    }

    return false;
}

void launchIde(std::ifstream& config_file,
               std::filesystem::path& file_path) {
    char isActive =
        (char)std::stoi(config::extractConfig(
            config_file, publicConfigActiveIDE_string, true,
            publicConfigErrorNoIsActiveIde));
    if (!isActive)
        return;

    std::string ide_string = config::extractConfig(
        config_file, publicConfigChosenIDE_string, true,
        publicConfigErrorNoIdeString);
    outputString("Launch IDE Command : ",
                              ide_string);

    std::string command =
        ide_string + " \"" + file_path.string() + "\"";
    std::cout << std::flush;
    std::cerr << std::flush;

    system(command.c_str());
}
