#define _WIN32_WINNT 0x0501
#include <iostream>
#include <string>
#include <asio.hpp>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

#include "yaml-cpp/yaml.h"

#include "./rippy.hpp"
#include "./cmdline.hpp"

struct domainEntry {
    std::string domain;
    std::vector<std::string> start_pages;
    std::vector<std::string> avoid;
    std::vector<std::string> rules;
};

int main(int argc, char* argv[])
{
    if (parseArgs(argc,argv))
        return 0;

    // check for project.yml in current directory
    if (!std::filesystem::exists("project.yml")) {
        std::cout << "QUIT: No project.yaml found in current directory.\r";
        return 0;
    }

    std::vector<std::string> visitedPages;

    /* config variables */
    std::string userAgent;
    int threads, depth;
    bool saveSession;
    std::vector<domainEntry> domains;
    domains.reserve(1);

    std::cout << "Starting Rippy (" << VERSION << ") - Vortex Interactive\n";

    /* load config */
    YAML::Node config = YAML::LoadFile("project.yml");
    userAgent = config["userAgent"].as<std::string>();
    threads = config["threads"].as<int>();
    depth = config["depth"].as<int>();
    saveSession = config["saveSession"].as<bool>();

    // the "domains" node is a sequence
    for (std::size_t i=0;i<config["domains"].size();i++) {
        domainEntry entry;
        entry.domain = config["domains"][i]["domain"].as<std::string>();
        for (std::size_t j=0;j<config["domains"][i]["start_pages"].size();j++) {
            entry.start_pages.emplace_back(config["domains"][i]["start_pages"][j].as<std::string>());
        }

        domains.emplace_back(entry);
    }
    std::cout << userAgent << " will run on (" << domains.size() << ") domains using " << threads << " threads, with ";
    if (depth)
        std::cout << "a depth of " << depth << "\n";
    else
        std::cout << "no depth limit\n";

    // check if last session was saved

    if (saveSession && std::filesystem::exists("session.yml")) {
        std::cout << "Found session.yml, load it? (Y/N): ";
        if (std::tolower(std::cin.get()) == 'y') {
            std::cout << "Loading session.yml\n";
            YAML::Node session = YAML::LoadFile("session.yml");
            // session.yml format:
            // visitedPages:
            //   - en.wikipedia.org/wiki/Main_Page
            for (YAML::const_iterator it = session["visitedPages"].begin(); it != session["visitedPages"].end(); ++it) {
                visitedPages.push_back(it->as<std::string>());
            }
        }
    }





    return 0;
}
