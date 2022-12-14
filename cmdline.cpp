#include "./cmdline.hpp"

#include <iostream>
#include <fstream>

const char* help = R""""(
Rippy is a command-line based tool for scraping webpages.
Usage:
rippy help - Display this help message
rippy create <name> - Create a new project (project.json) containing the settings and HTML rules for scraping each domain
rippy run - Run the project in the current directory.

The project.json rules include:
    1. The user agent to use when scraping, this is used to identify the scraper to the server
    2. The list of domains to recursively scrape, and the rules for scraping each domain
    3. The HTML tags and their attributes to scrape for each domain
    4. The output txt file's name.
)"""";

// The initial project.json file that is created when the user runs "create"
const char* defaultProjectConfig = R""""(
{
    "userAgent": "Rippy/1.0",
    "domains": [
        {
            "domain": "example.com",
            "rules": [
                {
                    "tag": "a",
                    "attribute": "href"
                }
            ]
        }
    ],
    "output": "output.txt"
}
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
    // for the project, and then create a file called project.json with the
    // project name in it.
    else if (arg1 == "create")
    {
        if (argc == 2)
        {
            std::cout << "Please specify a project name.\r";
            return 1;
        }
        std::string arg2 = argv[2];
        // add defaultProjectConfig to project.json 
        std::ofstream file("project.json");
        if (file.fail())
        {
            std::cout << "Error creating project.json\r";
            return 1;
        }
        file << defaultProjectConfig;
        file.close();
        std::cout << "Created project.json\r";
        return 1;
    }
    // If the user asked to run the project, return 0 to indicate that we should
    // continue to run the project.
    else if (arg1 == "run")
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