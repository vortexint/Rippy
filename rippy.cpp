#include "rippy.hpp"
#include <memory>

void rippyTask(asio::io_context& io_context, const LinkBuffer&, const rippyConfig& config)
{
    std::cout << "Started thread " << std::this_thread::get_id() << "\n";
}