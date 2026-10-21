#include "counters.h"

#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "usage: counters <data dir>\n";
        return 1;
    }

    try {
        std::map<std::string, std::vector<Break>> report = checkDirectory(argv[1]);

        for (const auto &file : report) {
            for (const Break &b : file.second) {
                std::cout << file.first << " channel " << b.channel
                          << " expected " << b.expected << " actual " << b.actual << "\n";
            }
        }

        std::cout << "\n";
        for (const auto &file : report)
            std::cout << file.first << ": " << file.second.size() << " breaks\n";
    } catch (const std::exception &e) {
        std::cout << e.what() << "\n";
        return 1;
    }
    return 0;
}
