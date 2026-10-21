#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

std::map<std::string, int> countMacs(const std::string &fileName);
std::vector<std::pair<std::string, int>> sortByCount(const std::map<std::string, int> &counts);
