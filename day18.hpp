#ifndef AOC2024_DAY18_HPP
#define AOC2024_DAY18_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <vector>

struct Coord {
    int x;
    int y;
    Coord(int _x, int _y) : x(_x), y(_y) {}
};
std::vector<Coord> parseInputFile() {
    std::ifstream input("../input/day18.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<Coord> result;
    int number1, number2;
    while (input >> number1) {
        input.ignore();
        input >> number2;
        result.emplace_back(number1, number2);
    }

    return result;
}

using Grid = std::array<std::array<char, 71>, 71>;

void print(const Grid& grid) {
    for (const auto& row : grid) {
        for (const auto& p : row) {
            std::cout << p;
        }
        std::cout << '\n';
    }
}

auto part1() {
    std::vector<Coord> data = parseInputFile();
    Grid grid;

    for (auto& row : grid) {
        for (auto& p : row) {
            p = '.';
        }
    }

    for (int i = 0; i < 1024; i++) {
        grid[data[i].y][data[i].x] = '#';
    }

    std::cout << '\n';
    print(grid);

    return 0;
}

auto part2() {
    std::vector<Coord> data = parseInputFile();

    return 0;
}

#endif // AOC2024_DAY18_HPP
