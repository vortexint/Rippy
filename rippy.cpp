#include "rippy.hpp"
#include <memory>

void rippyTask(asio::io_context& io_context, const LinkBuffer&, const rippyConfig& config)
{
    std::cout << "thread (" << std::this_thread::get_id() << ") started.\n";
}

// linkbuffer definitions
void LinkBuffer::checkAdd(const std::string& link)
{
}

std::string LinkBuffer::getNext()
{
    return "";
}

bool LinkBuffer::isEmpty()
{
    return true;
}

size_t LinkBuffer::size()
{
    return 0;
}

size_t LinkBuffer::visitedSize()
{
    return 0;
}
