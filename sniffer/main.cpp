#include "sniffer.h"

#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "usage: sniffer <log file>\n";
        return 1;
    }

    try {
        for (const auto &item : sortByCount(countMacs(argv[1])))
            std::cout << item.first << " " << item.second << "\n";
    } catch (const std::exception &e) {
        std::cout << e.what() << "\n";
        return 1;
    }
    return 0;
}
