#include "hills.h"

#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 5) {
        std::cout << "usage: rollercoaster <input file> <output file> <min width> <max width>\n";
        return 1;
    }

    try {
        int minWidth = std::stoi(argv[3]);
        int maxWidth = std::stoi(argv[4]);
        std::vector<std::pair<int, int>> hills = findHills(readFile(argv[1]), minWidth, maxWidth);

        std::ofstream out(argv[2]);
        if (!out) {
            std::cout << "cannot open " << argv[2] << "\n";
            return 1;
        }
        for (const auto &hill : hills)
            out << hill.first << " " << hill.second << "\n";

        std::cout << "found " << hills.size() << " hills\n";
    } catch (const std::exception &e) {
        std::cout << e.what() << "\n";
        return 1;
    }
    return 0;
}
