#define _WIN32_WINNT 0x0501
#include <iostream>
#include <string>
#include <asio.hpp>
#include <fstream>
#include <filesystem>

#include "./cmdline.hpp"

/* Rippy is a command-line based tool for scraping webpages.
Usage:
rippy help - Display this help message
rippy create <name> - Create a new project (project.yml) containing the settings and HTML rules for scraping each domain
*/

int main(int argc, char* argv[])
{
    if (parseArgs(argc,argv))
        return 0;

    // check for project.yml in current directory
    if (!std::filesystem::exists("project.yml")) {
        std::cout << "QUIT: No project.yaml found in current directory.\r";
        return 0;
    }

    


    return 0;
}
