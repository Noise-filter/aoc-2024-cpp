#ifndef DAY8_HPP
#define DAY8_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

constexpr int GRID_SIZE = 50;

struct Coord {
    int x;
    int y;

    Coord(int x, int y) : x(x), y(y) {}

    int convertToInt() const { return x + (y * GRID_SIZE); }

    friend bool operator==(const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator!=(const Coord& lhs, const Coord& rhs) { return !(lhs == rhs); }
    friend Coord operator-(const Coord& lhs, const Coord& rhs) { return Coord{lhs.x - rhs.x, lhs.y - rhs.y}; }
    friend Coord operator+(const Coord& lhs, const Coord& rhs) { return Coord{lhs.x + rhs.x, lhs.y + rhs.y}; }
    friend Coord& operator+=(Coord& lhs, const Coord& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }
};

std::map<char, std::vector<Coord>> parseInputFile() {
    std::ifstream input("../input/day8.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::map<char, std::vector<Coord>> result;
    std::string line;
    int y = 0;
    while (std::getline(input, line)) {
        for (int x = 0; x < line.size(); x++) {
            if (line[x] != '.') {
                auto it = result.find(line[x]);
                if (it == result.end()) {
                    result.emplace(line[x], std::vector<Coord>{Coord{x, y}});
                } else {
                    it->second.emplace_back(x, y);
                }
            }
        }
        ++y;
    }

    return result;
}

bool isWithinBounds(const Coord& coord) {
    return coord.x >= 0 && coord.x < GRID_SIZE && coord.y >= 0 && coord.y < GRID_SIZE;
}

auto part1() {
    std::map<char, std::vector<Coord>> data = parseInputFile();

    std::set<int> uniqueFrequencies;
    for (auto it = data.begin(); it != data.end(); ++it) {
        for (const auto& coord1 : it->second) {
            for (const auto& coord2 : it->second) {
                if (coord1 != coord2) {
                    auto diff = coord1 - coord2;
                    auto frequency = coord1 + diff;
                    if (isWithinBounds(frequency)) {
                        uniqueFrequencies.insert(frequency.convertToInt());
                    }
                }
            }
        }
    }

    return uniqueFrequencies.size();
}

auto part2() {
    std::map<char, std::vector<Coord>> data = parseInputFile();

    std::set<int> uniqueFrequencies;
    for (auto it = data.begin(); it != data.end(); ++it) {
        for (const auto& coord1 : it->second) {
            for (const auto& coord2 : it->second) {
                if (coord1 != coord2) {
                    auto diff = coord1 - coord2;
                    auto frequency = coord1 - diff;
                    while (isWithinBounds(frequency)) {
                        uniqueFrequencies.insert(frequency.convertToInt());
                        frequency += diff;
                    }
                }
            }
        }
    }

    return uniqueFrequencies.size();
}

#endif // DAY8_HPP
