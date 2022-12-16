#pragma once

#define _WIN32_WINNT 0x0601
#define VERSION "1.0"

#include <thread>
#include <asio.hpp>
#include <fstream>
#include <string>

struct domainRule {
    std::string tag, attribute, has;
};

struct domainEntry {
    std::string domain;
    std::vector<std::string> start_pages, avoid;
    std::vector<domainRule> rules;
};

class rippy {
public:
    rippy(asio::io_context& io_context, const std::string& user_agent, const std::vector<std::string>& visited_pages, const std::vector<domainEntry>& domains, int depth);
    void start();

private:
    asio::io_context& io_context;
    std::string user_agent;
    std::vector<std::string> visited_pages;
    std::vector<domainEntry> domains;
    int depth;
};