#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

struct Break
{
    int channel;
    uint32_t expected;
    uint32_t actual;
};

std::vector<Break> checkFile(const std::string &fileName);
std::map<std::string, std::vector<Break>> checkDirectory(const std::string &dir);
