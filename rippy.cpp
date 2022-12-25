#include "rippy.hpp"
#include <memory>

#pragma comment(lib, "Ws2_32.lib")
#include "HTTPRequest.hpp"

void rippyTask(LinkBuffer& lb, const rippyConfig& config)
{
    std::cout << "thread (" << std::this_thread::get_id() << ") started.\n";

    std::string link = lb.getNext();

    try {
        http::Request request(link);
        const auto response = request.send("GET");
        std::cout << std::string{response.body.begin(), response.body.end()} << '\n'; // print the result
        // save to response.txt (create if not exists, overwrite if exists)
        std::ofstream responseFile("response.txt", std::ios::out | std::ios::trunc);
        responseFile << std::string{response.body.begin(), response.body.end()};
        responseFile.close();



    }
    catch (const std::exception& e)
    {
        std::cerr << "Request failed, error: " << e.what() << '\n';
    }
}

// linkbuffer definitions
void LinkBuffer::addToQueue(std::string_view link)
{
    // add to unvisited_links
    unvisited_links.push(link.data());
}

std::string LinkBuffer::getNext()
{
    // get the next link from the queue
    std::string link = unvisited_links.front();
    unvisited_links.pop();
    return link;
}


size_t LinkBuffer::visitedSize()
{
    return visited_links.size();
}

size_t LinkBuffer::queueSize()
{
    return unvisited_links.size();
}