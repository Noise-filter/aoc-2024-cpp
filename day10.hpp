#ifndef AOC2024_DAY10_HPP
#define AOC2024_DAY10_HPP

#include <fstream>
#include <set>
#include <vector>

constexpr int GRID_SIZE = 50;

struct Coord {
    int x;
    int y;
    Coord(int x, int y) : x(x), y(y) {}

    int convertToInt() const { return x + y * GRID_SIZE; }
};

int findHikingPathsPart1(const std::vector<std::vector<int>>& data, std::set<int>& foundEndPositions, const Coord& pos,
                         int currentHeight);
int findHikingPathsPart2(const std::vector<std::vector<int>>& data, const Coord& pos, int currentHeight);

std::vector<Coord> findStartPositions(const std::vector<std::vector<int>>& data);
std::vector<std::vector<int>> parseInputFile() {
    std::ifstream input("../input/day10.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::vector<int>> result;
    result.reserve(GRID_SIZE);
    std::string temp;
    while (std::getline(input, temp)) {
        std::vector<int> line;
        line.reserve(GRID_SIZE);
        for (auto c : temp) {
            line.push_back(static_cast<int>(c - '0'));
        }
        result.push_back(line);
    }

    return result;
}

auto part1() {
    std::vector<std::vector<int>> data = parseInputFile();

    std::vector<Coord> startPositions = findStartPositions(data);

    long long result = 0;
    for (const auto& startPos : startPositions) {
        std::set<int> foundEndPositions;
        result += findHikingPathsPart1(data, foundEndPositions, startPos, 0);
    }

    return result;
}

std::vector<Coord> findStartPositions(const std::vector<std::vector<int>>& data) {
    std::vector<Coord> startPositions;
    for (int y = 0; y < data.size(); y++) {
        for (int x = 0; x < data[y].size(); x++) {
            if (data[y][x] == 0) {
                startPositions.emplace_back(x, y);
            }
        }
    }
    return startPositions;
}

int findHikingPathsPart1(const std::vector<std::vector<int>>& data, std::set<int>& foundEndPositions, const Coord& pos,
                         int currentHeight) {
    if (pos.x < 0 || pos.x >= data.size() || pos.y < 0 || pos.y >= data.size()) {
        return 0;
    }
    if (data[pos.y][pos.x] == currentHeight) {
        if (currentHeight == 9) {
            int posAsInt = pos.convertToInt();
            if (foundEndPositions.find(posAsInt) == foundEndPositions.end()) {
                foundEndPositions.insert(posAsInt);
                return 1;
            } else {
                return 0;
            }
        }

        currentHeight += 1;
        return findHikingPathsPart1(data, foundEndPositions, Coord{pos.x + 1, pos.y}, currentHeight) +
               findHikingPathsPart1(data, foundEndPositions, Coord{pos.x - 1, pos.y}, currentHeight) +
               findHikingPathsPart1(data, foundEndPositions, Coord{pos.x, pos.y + 1}, currentHeight) +
               findHikingPathsPart1(data, foundEndPositions, Coord{pos.x, pos.y - 1}, currentHeight);
    }

    return 0;
}

auto part2() {
    std::vector<std::vector<int>> data = parseInputFile();

    std::vector<Coord> startPositions = findStartPositions(data);

    long long result = 0;
    for (const auto& startPos : startPositions) {
        result += findHikingPathsPart2(data, startPos, 0);
    }

    return result;
}

int findHikingPathsPart2(const std::vector<std::vector<int>>& data, const Coord& pos, int currentHeight) {
    if (pos.x < 0 || pos.x >= data.size() || pos.y < 0 || pos.y >= data.size()) {
        return 0;
    }
    if (data[pos.y][pos.x] == currentHeight) {
        if (currentHeight == 9) {
            return 1;
        }

        currentHeight += 1;
        return findHikingPathsPart2(data, Coord{pos.x + 1, pos.y}, currentHeight) +
               findHikingPathsPart2(data, Coord{pos.x - 1, pos.y}, currentHeight) +
               findHikingPathsPart2(data, Coord{pos.x, pos.y + 1}, currentHeight) +
               findHikingPathsPart2(data, Coord{pos.x, pos.y - 1}, currentHeight);
    }

    return 0;
}

#endif // AOC2024_DAY10_HPP
