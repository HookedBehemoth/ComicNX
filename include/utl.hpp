#include <string>
#include <chrono>
#include "model.hpp"
namespace utl {
    std::string getRelativeTime(uint64_t timestamp);
    model::CLang getLanguage(std::string langName);
}