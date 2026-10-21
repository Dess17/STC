#include "counters.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

std::vector<Break> checkFile(const std::string &fileName)
{
    std::vector<Break> breaks;
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "cannot open " << fileName << "\n";
        return breaks;
    }

    uint32_t last[32] = {};
    bool started[32] = {};
    unsigned char b[4];
    int channel = 0;
    while (file.read(reinterpret_cast<char *>(b), 4)) {
        uint32_t value = b[0] | b[1] << 8 | b[2] << 16 | uint32_t(b[3]) << 24;
        if (started[channel] && value != last[channel] + 1)
            breaks.push_back({channel, last[channel] + 1, value});
        last[channel] = value;
        started[channel] = true;
        channel = (channel + 1) % 32;
    }
    return breaks;
}

std::map<std::string, std::vector<Break>> checkDirectory(const std::string &dir)
{
    std::vector<std::filesystem::path> files;
    for (const auto &entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().filename().string()[0] != '.')
            files.push_back(entry.path());
    }

    std::vector<std::vector<Break>> results(files.size());
    std::vector<std::thread> threads;
    for (size_t i = 0; i < files.size(); i++)
        threads.emplace_back([&files, &results, i] { results[i] = checkFile(files[i].string()); });
    for (std::thread &t : threads)
        t.join();

    std::map<std::string, std::vector<Break>> report;
    for (size_t i = 0; i < files.size(); i++)
        report[files[i].filename().string()] = results[i];
    return report;
}
