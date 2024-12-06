#ifndef AOC2024_DAY6_HPP
#define AOC2024_DAY6_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

constexpr char WALL = '#';
constexpr int SIZE = 130;

std::vector<std::vector<char>> parseInputFile() {
    std::ifstream input("../input/day6.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::vector<char>> data;
    data.reserve(130);

    std::string buffer;
    buffer.reserve(130);
    while (std::getline(input, buffer)) {
        data.emplace_back(buffer.begin(), buffer.end());
    }

    return data;
}

struct Pos {
    int x = 0;
    int y = 0;

    int convertToInt() const { return y * SIZE + x; }

    friend Pos operator+(Pos lhs, const Pos& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    friend bool operator==(const Pos& lhs, const Pos& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator!=(const Pos& lhs, const Pos& rhs) { return !(lhs == rhs); }
    friend bool operator<(const Pos& lhs, const Pos& rhs) { return lhs.convertToInt() < rhs.convertToInt(); }
};

struct Dir {
    int dir = 0;

    void rotate() {
        dir += 1;
        dir %= 4;
    }

    Pos getDirectionPosDiff() const {
        if (dir == 0) {
            return Pos{0, -1};
        } else if (dir == 1) {
            return Pos{1, 0};
        } else if (dir == 2) {
            return Pos{0, 1};
        } else if (dir == 3) {
            return Pos{-1, 0};
        }
        return Pos{0, 0};
    }
};

struct Guard {
    Pos pos;
    Dir dir;
};

inline bool operator<(const Guard& lhs, const Guard& rhs) {
    return (lhs.pos.convertToInt() + (lhs.dir.dir * 100000)) < (rhs.pos.convertToInt() + (rhs.dir.dir * 100000));
}

Pos findGuard(const std::vector<std::vector<char>>& data) {
    for (int y = 0; y < data.size(); y++) {
        for (int x = 0; x < data[y].size(); x++) {
            if (data[y][x] == '^') {
                return Pos{x, y};
            }
        }
    }
    return Pos{-1, -1};
}

bool insideMap(Pos guardPos, const std::vector<std::vector<char>>& data) {
    return guardPos.x >= 0 && guardPos.x < data.size() && guardPos.y >= 0 && guardPos.y < data.size();
}

bool isWall(Pos pos, const std::vector<std::vector<char>>& data) { return data[pos.y][pos.x] == WALL; }

auto part1() {
    std::vector<std::vector<char>> data = parseInputFile();

    std::set<int> distinctPositions;
    Guard guard{findGuard(data), Dir{0}};

    while (insideMap(guard.pos, data)) {
        distinctPositions.insert(guard.pos.convertToInt());
        Pos diff = guard.dir.getDirectionPosDiff();
        Pos newPos = guard.pos + diff;
        if (insideMap(newPos, data) && isWall(newPos, data)) {
            guard.dir.rotate();
        } else {
            guard.pos = newPos;
        }
    }

    return distinctPositions.size();
}

bool isLoop(const std::vector<std::vector<char>> data) {
    std::set<Guard> distinctPositions;
    Guard guard{findGuard(data), Dir{0}};

    while (insideMap(guard.pos, data)) {
        if (distinctPositions.find(guard) != distinctPositions.end()) {
            return true;
        }
        distinctPositions.insert(guard);
        Pos diff = guard.dir.getDirectionPosDiff();
        Pos newPos = guard.pos + diff;
        if (insideMap(newPos, data) && isWall(newPos, data)) {
            guard.dir.rotate();
        } else {
            guard.pos = newPos;
        }
    }
    return false;
}

auto part2() {

    std::vector<std::vector<char>> data = parseInputFile();

    auto guardStartPos = findGuard(data);
    std::set<Pos> distinctPositions;
    Guard guard{guardStartPos, Dir{0}};

    while (insideMap(guard.pos, data)) {
        distinctPositions.insert(guard.pos);
        Pos diff = guard.dir.getDirectionPosDiff();
        Pos newPos = guard.pos + diff;
        if (insideMap(newPos, data) && isWall(newPos, data)) {
            guard.dir.rotate();
        } else {
            guard.pos = newPos;
        }
    }

    std::set<int> uniqueWallPositions;
    for (const auto& guardPosition : distinctPositions) {
        if (guardPosition != guardStartPos) {
            data[guardPosition.y][guardPosition.x] = WALL;
            if (isLoop(data)) {
                uniqueWallPositions.insert(guardPosition.convertToInt());
            }
            data[guardPosition.y][guardPosition.x] = '.';
        }
    }

    return uniqueWallPositions.size();
}

#endif // AOC2024_DAY6_HPP
