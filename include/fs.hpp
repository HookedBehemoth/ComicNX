#pragma once

#include <switch.h>
#include <string>
#include <vector>
#include <dirent.h>
#include <stdio.h>
#include "model.hpp"
namespace fs{
    u64 getFreeSpace();
    std::string getHumanReadableBytes(u64);
    std::vector<std::string> listDirectory(std::string);
    bool writeFile(std::string filename, std::string data);
    bool deleteFile(std::string filename);
    bool fileExists(std::string filename);
    bool mkpath(std::string path);
    std::string getSuffix(int fType);
    std::string getFlagPath(int cLang);
}