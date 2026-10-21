#include "sniffer.h"

#include <algorithm>
#include <fstream>
#include <set>
#include <sstream>
#include <stdexcept>

static bool isAddressKey(const std::string &key)
{
    std::stringstream parts(key);
    std::string part;
    while (std::getline(parts, part, '/')) {
        if (part == "RA" || part == "TA" || part == "SA")
            return true;
    }
    return false;
}

std::map<std::string, int> countMacs(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("cannot open " + fileName);

    std::map<std::string, int> counts;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::set<std::string> macs;
        std::stringstream fields(line.substr(line.find('\t') + 1));
        std::string field;
        while (std::getline(fields, field, ',')) {
            size_t eq = field.find('=');
            if (eq != std::string::npos && isAddressKey(field.substr(0, eq)))
                macs.insert(field.substr(eq + 1));
        }

        for (const std::string &mac : macs)
            counts[mac]++;
    }
    return counts;
}

std::vector<std::pair<std::string, int>> sortByCount(const std::map<std::string, int> &counts)
{
    std::vector<std::pair<std::string, int>> result(counts.begin(), counts.end());
    std::stable_sort(result.begin(), result.end(), [](const auto &a, const auto &b) {
        return a.second > b.second;
    });
    return result;
}
