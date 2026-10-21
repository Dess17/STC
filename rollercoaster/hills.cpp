#include "hills.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iterator>
#include <stdexcept>

std::vector<float> readFile(const std::string &fileName)
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file)
        throw std::runtime_error("cannot open " + fileName);

    std::vector<unsigned char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::vector<float> data(bytes.size() / 4);
    for (size_t i = 0; i < data.size(); i++) {
        const unsigned char *b = &bytes[i * 4];
        uint32_t value = b[0] | b[1] << 8 | b[2] << 16 | uint32_t(b[3]) << 24;
        std::memcpy(&data[i], &value, 4);
    }
    return data;
}

static double median(std::vector<double> values)
{
    size_t middle = values.size() / 2;
    std::nth_element(values.begin(), values.begin() + middle, values.end());
    double result = values[middle];
    if (values.size() % 2 == 0)
        result = (result + *std::max_element(values.begin(), values.begin() + middle)) / 2;
    return result;
}

std::vector<std::pair<int, int>> findHills(const std::vector<float> &data, int minWidth, int maxWidth)
{
    std::vector<std::pair<int, int>> hills;
    if (data.empty())
        return hills;

    const int n = static_cast<int>(data.size());
    const int radius = 4;
    const int blockSize = 1024;
    const int neighbours = 8;

    std::vector<double> sum(n + 1, 0);
    for (int i = 0; i < n; i++)
        sum[i + 1] = sum[i] + data[i];

    std::vector<double> smooth(n);
    for (int i = 0; i < n; i++) {
        int from = std::max(0, i - radius);
        int to = std::min(n, i + radius + 1);
        smooth[i] = (sum[to] - sum[from]) / (to - from);
    }

    int blocks = (n + blockSize - 1) / blockSize;
    std::vector<double> blockLevel(blocks), blockNoise(blocks), blockCenter(blocks);
    for (int b = 0; b < blocks; b++) {
        int from = b * blockSize;
        int to = std::min(n, from + blockSize);
        std::vector<double> block(smooth.begin() + from, smooth.begin() + to);
        blockLevel[b] = median(block);

        double squares = 0;
        int count = 0;
        for (double v : block) {
            if (v < blockLevel[b]) {
                squares += (v - blockLevel[b]) * (v - blockLevel[b]);
                count++;
            }
        }
        blockNoise[b] = count > 0 ? std::sqrt(squares / count) : 0;
        blockCenter[b] = from + (to - from - 1) / 2.0;
    }

    double level = median(blockLevel);
    double noise = median(blockNoise);

    std::vector<double> x, y;
    for (int b = 0; b < blocks; b++) {
        std::vector<double> quiet;
        for (int k = std::max(0, b - neighbours); k < std::min(blocks, b + neighbours + 1); k++) {
            if (blockNoise[k] <= 1.5 * noise && blockLevel[k] <= level + 3 * noise)
                quiet.push_back(blockLevel[k]);
        }
        if (!quiet.empty()) {
            x.push_back(blockCenter[b]);
            y.push_back(median(quiet));
        }
    }

    std::vector<double> floorLevel(n, level);
    if (!x.empty()) {
        size_t j = 0;
        for (int i = 0; i < n; i++) {
            if (i <= x.front()) {
                floorLevel[i] = y.front();
            } else if (i >= x.back()) {
                floorLevel[i] = y.back();
            } else {
                while (x[j + 1] <= i)
                    j++;
                floorLevel[i] = (y[j + 1] - y[j]) / (x[j + 1] - x[j]) * (i - x[j]) + y[j];
            }
        }
    }

    int i = 0;
    while (i < n) {
        if (smooth[i] - floorLevel[i] <= noise) {
            i++;
            continue;
        }
        int left = i;
        double peak = 0;
        while (i < n && smooth[i] - floorLevel[i] > noise) {
            peak = std::max(peak, smooth[i] - floorLevel[i]);
            i++;
        }
        int right = i;
        int width = right - left;
        if (left > 0 && right < n && peak > 6 * noise && width >= minWidth && width <= maxWidth)
            hills.push_back({left, right});
    }
    return hills;
}
