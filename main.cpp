#include <filesystem>
#include <chrono>

#include "yaml-cpp/yaml.h"

#include "./rippy.hpp"
#include "./cmdline.hpp"

int main(int argc, char* argv[])
{
    // check if not in debug mode
    #ifndef _DEBUG
    if (parseArgs(argc,argv))
        return 0;
    #endif

    // check for project.yml in current directory
    if (!std::filesystem::exists("project.yml")) {
        std::cout << "QUIT: No project.yml found in current directory, run \"rippy help\" for more information\n";
        return 0;
    }

    /* config variables */
    rippyConfig config;
    LinkBuffer linkBuffer;

    std::cout << "Starting Rippy (" << VERSION << ") - Vortex Interactive\n\n";

    /* load project */
    YAML::Node project = YAML::LoadFile("project.yml");
    config.userAgent = project["userAgent"].as<std::string>();
    config.threads = project["threads"].as<int>();
    config.depth = project["depth"].as<int>();
    config.saveSession = project["saveSession"].as<bool>();

    // Validating the project.yml file
    try
    {
        // the "domains" node is a sequence
        for (std::size_t i=0;i<project["domains"].size();i++) {
            domainEntry entry;
            std::string domain = project["domains"][i]["domain"].as<std::string>();
            if (domain == "null") {
                std::clog << "QUIT: Domain " << i << " is empty\n";
                exit(1);
            }
            entry.domain = domain;

            // check if start_pages property exists for the domain
            if (!project["domains"][i]["start_pages"]) {
                std::clog << "NOTE: There is no \"start_pages\" list for the domain \"" << domain << "\", Rippy will use the domain as the start page.\n\n";
                entry.start_pages.emplace_back(domain);
            }
            else if (project["domains"][i]["start_pages"].size() == 0) {
                std::cerr << "NOTE: start_pages exists for domain \"" << domain << "\" but it has no pages, Rippy will use the domain as the start page.\n\n";
                entry.start_pages.emplace_back(domain);
            }
            else
            {
                for (std::size_t j=0;j<project["domains"][i]["start_pages"].size();j++) {
                    std::string page = project["domains"][i]["start_pages"][j].as<std::string>();
                    if (page == "null")
                        continue;
                    entry.start_pages.emplace_back(page);
                }
            }

            for (std::size_t j=0;j<project["domains"][i]["filter"].size();j++) {
                std::string filter = project["domains"][i]["filter"][j].as<std::string>();
                if (filter == "null")
                    continue; // filter is optional
                entry.filter.emplace_back(filter);
            }

            // check if filter_mode property exists for the domain
            if (!project["domains"][i]["filter_mode"]) {
                std::cerr << "NOTE: There is no \"filter_mode\" property for domain \"" << domain << "\", Rippy will use \"whitelist\" mode.\n\n";
                entry.filter_mode = "whitelist";
            }
            else {
                std::string filter_mode = project["domains"][i]["filter_mode"].as<std::string>();
                if (filter_mode == "null") {
                    std::cerr << "NOTE: \"filter_mode\" is empty for domain \"" << domain << "\", Rippy will use \"whitelist\" mode.\n\n";
                    entry.filter_mode = "whitelist";
                }
                else if (filter_mode != "whitelist" && filter_mode != "blacklist") {
                    std::cerr << "NOTE: \"filter_mode\" is invalid for domain \"" << domain << "\", Rippy will use \"whitelist\" mode.\n\n";
                    entry.filter_mode = "whitelist";
                }
                else {
                    entry.filter_mode = filter_mode;
                }
            }

            // check if rules property exists for the domain
            if (!project["domains"][i]["rules"]) {
                std::cerr << "QUIT: There is no \"rules\" property for domain \"" << domain << "\"\n";
                exit(1);
            }

            for (std::size_t j=0;j<project["domains"][i]["rules"].size();j++) {
                domainRule rule;
                std::string tag = project["domains"][i]["rules"][j]["tag"].as<std::string>();
                if (tag == "null") {
                    std::cerr << "QUIT: \"tag\" is empty on rule " << j << " for domain \"" << domain << "\"\n";
                    exit(1);
                }
                rule.tag = tag;

                std::string attribute = project["domains"][i]["rules"][j]["attribute"].as<std::string>();
                if (attribute == "null") {
                    std::cerr << "QUIT: \"attribute\" is empty on rule " << j << " for domain \"" << domain << "\"\n";
                    exit(1);
                }
                rule.attribute = attribute;

                std::string value = project["domains"][i]["rules"][j]["value"].as<std::string>();
                if (value == "null") {
                    std::cerr << "QUIT: \"value\" is empty on rule " << j << " for domain \"" << domain << "\"\n";
                    exit(1);
                }
                entry.rules.emplace_back(rule);
            }
            config.domains.emplace_back(entry);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "QUIT: Undocumented error while parsing project.yml: " << e.what() << '\n';
        exit(1);
    }
    std::cout << config.userAgent << " will run on " << config.domains.size() << " domains using " << config.threads << " threads, with ";
    if (config.depth)
        std::cout << "a depth of " << config.depth << " and\n";
    else
        std::cout << "no depth limit and\n";
    if (config.saveSession)
        std::cout << "session saving enabled.\n\n";
    else
        std::cout << "session saving disabled.\n\n";

    /* check if last session was saved */

    if (config.saveSession && std::filesystem::exists("session.yml")) {
        std::cout << "\nFound session.yml, restore previously crawled pages? (Y/N): ";
        if (std::tolower(std::cin.get()) == 'y') {
            std::cout << "Loading session.yml\n";
            YAML::Node session = YAML::LoadFile("session.yml");
            for (YAML::const_iterator it = session["visitedPages"].begin(); it != session["visitedPages"].end(); ++it) {
                config.visitedPages.push_back(it->as<std::string>());
            }
        }
    }

    // check if amount of threads is valid
    if (config.threads > std::thread::hardware_concurrency()) {
        std::cout << "QUIT: Number of threads is higher than hardware supported which is" << std::thread::hardware_concurrency() << ".\n";
        return 0;
    }

    auto startTime = std::chrono::high_resolution_clock::now();

    asio::io_context io_context;

    // void rippyTask(asio::io_context& io_context, const LinkBuffer&, const rippyConfig& config);
    // create a pool of threads to run rippyTask and wait on a while loop while the threads are running
    std::vector<std::thread> threads(config.threads);
    for (std::size_t i=0;i<config.threads;i++) {
        threads[i] = std::thread(rippyTask, std::ref(io_context), std::ref(linkBuffer), std::ref(config));
    }

    while (true) {
        // check if threads are still running, if not, break the loop
        if (std::all_of(threads.begin(), threads.end(), [](std::thread& t) { return !t.joinable(); })) {
            break;
        }

        std::cout << "Crawling in progress.";
    }


    return 0;
}
