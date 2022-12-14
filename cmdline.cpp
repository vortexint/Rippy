#include "./cmdline.hpp"

#include <iostream>
#include <fstream>

const char* help = R""""(
Rippy is a command-line based tool for scraping webpages.
Usage:
rippy help - Display this help message
rippy create <name> - Create a new project (project.yml) containing the settings and HTML rules for scraping each domain
rippy start - Run the project in the current directory.
)"""";

// The initial project.yml file that is created when the user runs "create"
const char* defaultProjectConfig = R""""(
# Crawler configuration  (YAML)
# The user agent to use when scraping, this is used to identify the scraper to the server.
userAgent: Rippy/1.0
threads: 4 # Increasing this will increase the speed of the scraper, but will also increase the load on the server.
depth: 0 # disable depth limit, (e.g. 10000 would limit the scraper to 10000 pages)
saveSession: true # save the session to a file, so that it can be resumed if the program is interrupted
domains:
  - domain: en.wikipedia.com
    start_pages:
      - /wiki/Main_Page
    avoid:
      - /w/index.php?title=Special
    rules:
      - tag: span
        attribute: class
        has: mw-page-title-main
      - tag: div
        attribute: id
        has: mw-content-text
output: output.txt
)"""";

// Returns 1 if the program should exit, 0 only if the user used the "run" command
int parseArgs(int argc, char* argv[])
{
    // If user ran the program without any arguments, print the help message
    if (argc == 1)
    {
        std::cout << help;
        return 1;
    }

    // Extract the first argument
    std::string arg1 = argv[1];
    // If the user asked for help, print the help message
    if (arg1 == "help")
    {
        std::cout << help;
        return 1;
    }
    // If the user asked to create a new project, check that they supplied a name
    // for the project, and then create a file called project.yml with the
    // project name in it.
    else if (arg1 == "create")
    {
        if (argc == 2)
        {
            std::cout << "Please specify a project name.\r";
            return 1;
        }
        std::string arg2 = argv[2];
        // add defaultProjectConfig to project.yml 
        std::ofstream file("project.yml");
        if (file.fail())
        {
            std::cout << "Error creating project.yml\r";
            return 1;
        }
        file << defaultProjectConfig;
        file.close();
        std::cout << "Created project.yml\r";
        return 1;
    }
    // If the user asked to run the project, return 0 to indicate that we should
    // continue to run the project.
    else if (arg1 == "start")
    {
        return 0;
    }
    // If the user supplied an unknown command, print an error message
    else
    {
        std::cout << "Unknown command: " << arg1 << "\r";
        return 1;
    }
}