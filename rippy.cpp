#include "rippy.hpp"
#include <memory>

void rippyTask(asio::io_context& io_context, const LinkBuffer&, const rippyConfig& config)
{
    std::cout << "thread (" << std::this_thread::get_id() << ") started.\n";
    
}

// linkbuffer definitions
void LinkBuffer::addToQueue(std::string_view link)
{
    // add to unvisited_links
    unvisited_links.push(link.data());
}

std::string LinkBuffer::getNext()
{
    return "";
}


size_t LinkBuffer::size()
{
    return visited_links.size();
}
