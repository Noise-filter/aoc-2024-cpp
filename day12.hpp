#ifndef AOC2024_DAY12_HPP
#define AOC2024_DAY12_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

int gridSize = 0;

enum class Direction {
    NORTH = 0,
    EAST = 1,
    SOUTH = 2,
    WEST = 3,
    COUNT = 4,
};

constexpr int DIRECTION_COUNT = static_cast<int>(Direction::COUNT);
using Perimeter = std::bitset<DIRECTION_COUNT>;

void travelAlongEdge(const std::vector<char>& data, std::vector<Perimeter>& perimeters, int index, int directionAsInt,
                     int diffX, int diffY);

std::pair<int, int> convertDirectionToCoordDiff(Direction direction);

std::vector<char> parseInputFile() {
    std::ifstream input("../input/day12.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    input.seekg(0, std::ios::end);
    std::streamsize size = input.tellg();
    input.seekg(0);
    std::string buffer(size, ' ');
    input.read(&buffer[0], size);

    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.cend());

    gridSize = static_cast<int>(sqrt(buffer.size()));

    return {buffer.begin(), buffer.end()};
}

std::vector<Perimeter> calculatePerimeters(const std::vector<char>& data) {
    std::vector<Perimeter> result;
    result.reserve(data.size());

    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            char character = data[x + y * gridSize];
            Perimeter perimeter;
            if (x == 0) {
                perimeter.set(static_cast<int>(Direction::WEST), true);
                perimeter.set(static_cast<int>(Direction::EAST), data[x + 1 + y * gridSize] != character);
            } else if (x == gridSize - 1) {
                perimeter.set(static_cast<int>(Direction::EAST), true);
                perimeter.set(static_cast<int>(Direction::WEST), data[x - 1 + y * gridSize] != character);
            } else {
                perimeter.set(static_cast<int>(Direction::EAST), data[x + 1 + y * gridSize] != character);
                perimeter.set(static_cast<int>(Direction::WEST), data[x - 1 + y * gridSize] != character);
            }

            if (y == 0) {
                perimeter.set(static_cast<int>(Direction::NORTH), true);
                perimeter.set(static_cast<int>(Direction::SOUTH), data[x + (y + 1) * gridSize] != character);
            } else if (y == gridSize - 1) {
                perimeter.set(static_cast<int>(Direction::SOUTH), true);
                perimeter.set(static_cast<int>(Direction::NORTH), data[x + (y - 1) * gridSize] != character);
            } else {
                perimeter.set(static_cast<int>(Direction::SOUTH), data[x + (y + 1) * gridSize] != character);
                perimeter.set(static_cast<int>(Direction::NORTH), data[x + (y - 1) * gridSize] != character);
            }

            result.push_back(perimeter);
        }
    }

    return result;
}

void findArea(int x, int y, char character, std::vector<bool>& usedPositions, std::vector<int>& area,
              const std::vector<char>& data) {
    int index = x + y * gridSize;
    if (x < 0 || y < 0 || x >= gridSize || y >= gridSize) {
        return;
    }
    if (usedPositions[index]) {
        return;
    }

    if (data[index] == character) {
        usedPositions[index] = true;
        area.push_back(index);

        findArea(x + 1, y, character, usedPositions, area, data);
        findArea(x - 1, y, character, usedPositions, area, data);
        findArea(x, y + 1, character, usedPositions, area, data);
        findArea(x, y - 1, character, usedPositions, area, data);
    }
}

auto part1() {
    std::vector<char> data = parseInputFile();
    std::vector<Perimeter> perimeters = calculatePerimeters(data);

    std::vector<bool> usedPositions;
    usedPositions.resize(perimeters.size());
    std::vector<std::vector<int>> areas;

    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            int index = x + y * gridSize;
            if (!usedPositions[index]) {
                std::vector<int> area;
                findArea(x, y, data[index], usedPositions, area, data);
                areas.push_back(std::move(area));
            }
        }
    }

    size_t result = 0;
    for (const auto& area : areas) {
        size_t perimeter = 0;
        for (auto index : area) {
            perimeter += perimeters[index].count();
        }
        result += (area.size() * perimeter);
    }

    return result;
}

auto part2() {
    std::vector<char> data = parseInputFile();
    std::vector<Perimeter> perimeters = calculatePerimeters(data);

    std::vector<bool> usedPositions;
    usedPositions.resize(perimeters.size());
    std::vector<std::vector<int>> areas;

    for (int y = 0; y < gridSize; y++) {
        for (int x = 0; x < gridSize; x++) {
            int index = x + y * gridSize;
            if (!usedPositions[index]) {
                std::vector<int> area;
                findArea(x, y, data[index], usedPositions, area, data);
                std::sort(area.begin(), area.end());
                areas.push_back(std::move(area));
            }
        }
    }

    const auto directions =
        std::array<Direction, 4>{Direction::NORTH, Direction::SOUTH, Direction::WEST, Direction::EAST};

    size_t result = 0;
    for (const auto& area : areas) {
        size_t sides = 0;
        for (const auto index : area) {
            size_t count = perimeters[index].count();
            if (count > 0) {
                sides += count;
                for (auto direction : directions) {
                    int directionAsInt = static_cast<int>(direction);
                    if (perimeters[index].test(directionAsInt)) {
                        auto diff = convertDirectionToCoordDiff(direction);
                        travelAlongEdge(data, perimeters, index, directionAsInt, diff.first, diff.second);
                    }
                }
            }
        }
        result += area.size() * sides;
    }
    return result;
}

std::pair<int, int> convertDirectionToCoordDiff(Direction direction) {
    int diffX = 0;
    int diffY = 0;
    if (direction == Direction::NORTH || direction == Direction::SOUTH) {
        diffX = 1;
    } else {
        diffY = 1;
    }
    return {diffX, diffY};
}

void travelAlongEdge(const std::vector<char>& data, std::vector<Perimeter>& perimeters, int index, int directionAsInt,
                     int diffX, int diffY) {
    int y = index / gridSize;
    int x = index - y * gridSize;
    char character = data[index];
    int newIndex = x + y * gridSize;
    do {
        perimeters[newIndex].set(directionAsInt, false);
        x += diffX;
        y += diffY;
        newIndex = x + y * gridSize;
    } while (data[newIndex] == character && perimeters[newIndex].test(directionAsInt));
}

#endif // AOC2024_DAY12_HPP
