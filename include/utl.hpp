#include <string>
#include <chrono>
#include "model.hpp"
namespace utl {
    std::string getRelativeTime(uint64_t timestamp);
    bool isServiceRunning(const char *serviceName);
    bool canSwkbd();
}