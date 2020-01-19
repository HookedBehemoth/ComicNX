#pragma once

#include <switch.h>
#include "json.hpp"
#include <string>

bool canSwkbd();
u64 getInt(const nlohmann::json::value_type &idValue);
std::string getString(const nlohmann::json::value_type &strValue);
