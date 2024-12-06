#ifndef AOC2024_DAY4_HPP
#define AOC2024_DAY4_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

std::vector<std::string> parseInputFile() {
    std::ifstream input("../input/day4.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::string> data;
    data.reserve(140);

    std::string buffer;
    buffer.reserve(140);
    while (std::getline(input, buffer)) {
        data.emplace_back(buffer);
    }

    return data;
}

bool findXmas(const std::vector<std::string>& data, int xPos, int yPos, int xDiff, int yDiff) {
    static constexpr std::array<char, 4> xmasString{'X', 'M', 'A', 'S'};

    for (const char c : xmasString) {
        if (xPos < 0 || xPos >= data.size() || yPos < 0 || yPos >= data.size()) {
            return false;
        }
        if (data[yPos][xPos] != c) {
            return false;
        }
        xPos += xDiff;
        yPos += yDiff;
    }
    return true;
}

bool findMas(const std::vector<std::string>& data, int xPos, int yPos) {
    if (data[yPos][xPos] == 'A') {
        if ((data[yPos - 1][xPos - 1] == 'M' && data[yPos + 1][xPos + 1] == 'S' ||
             data[yPos - 1][xPos - 1] == 'S' && data[yPos + 1][xPos + 1] == 'M') &&
            (data[yPos - 1][xPos + 1] == 'M' && data[yPos + 1][xPos - 1] == 'S' ||
             data[yPos - 1][xPos + 1] == 'S' && data[yPos + 1][xPos - 1] == 'M')) {
            return true;
        }
    }
    return false;
}

auto part1() {
    int result = 0;

    std::vector<std::string> data = parseInputFile();

    for (int y = 0; y < data.size(); y++) {
        for (int x = 0; x < data[y].size(); x++) {
            for (int xDiff = -1; xDiff < 2; ++xDiff) {
                for (int yDiff = -1; yDiff < 2; ++yDiff) {
                    result += findXmas(data, x, y, xDiff, yDiff);
                }
            }
            // std::cout << data[y][x];
        }
        // std::cout << '\n';
    }

    return result;
}

auto part2() {
    int result = 0;

    std::vector<std::string> data = parseInputFile();
    for (int y = 1; y < data.size()-1; y++) {
        for (int x = 1; x < data[y].size()-1; x++) {
            result += findMas(data, x, y);
        }
        // std::cout << '\n';
    }

    return result;
}

#endif // AOC2024_DAY4_HPP
