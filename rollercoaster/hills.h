#pragma once

#include <string>
#include <utility>
#include <vector>

std::vector<float> readFile(const std::string &fileName);
std::vector<std::pair<int, int>> findHills(const std::vector<float> &data, int minWidth, int maxWidth);
