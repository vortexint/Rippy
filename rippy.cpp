#include "rippy.hpp"
#include <memory>
// definitions for rippy class
rippy::rippy(asio::io_context& io_context, const std::string& user_agent, const std::vector<std::string>& visited_pages, const std::vector<domainEntry>& domains, int depth) : io_context(io_context), user_agent(user_agent), visited_pages(visited_pages), domains(domains), depth(depth) {}

void rippy::start() {
    for (auto& domain : domains) {
        for (auto& page : domain.start_pages) {
            std::make_shared<Page>(io_context, user_agent, page, domain, depth, visited_pages)->start();
        }
    }
}