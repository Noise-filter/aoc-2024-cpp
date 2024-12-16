
#ifndef DAY15_HPP
#define DAY15_HPP

#include <algorithm>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

struct Coord {
    long long x;
    long long y;
    Coord(long long _x, long long _y) : x(_x), y(_y) {}

    friend Coord operator%(const Coord& lhs, const Coord& rhs) { return {lhs.x % rhs.x, lhs.y % rhs.y}; }
    friend Coord operator/(const Coord& lhs, const Coord& rhs) { return {lhs.x / rhs.x, lhs.y / rhs.y}; }
    friend Coord operator*(const Coord& lhs, const long long number) { return {lhs.x * number, lhs.y * number}; }
    friend Coord operator+(const Coord& lhs, const Coord& rhs) { return {lhs.x + rhs.x, lhs.y + rhs.y}; }
    friend Coord operator-(const Coord& lhs, const Coord& rhs) { return {lhs.x - rhs.x, lhs.y - rhs.y}; }
    friend Coord& operator+=(Coord& lhs, const Coord& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }
    friend Coord& operator-=(Coord& lhs, const Coord& rhs) {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        return lhs;
    }
    friend Coord& operator%=(Coord& lhs, const Coord& rhs) {
        lhs.x %= rhs.x;
        lhs.y %= rhs.y;
        return lhs;
    }
    friend bool operator==(const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator<(const Coord& lhs, const Coord& rhs) { return lhs.x < rhs.x && lhs.y < rhs.y; }
};

struct Robot {
    Coord pos;
    Coord vel;
    Robot(const Coord _pos, const Coord _vel) : pos(_pos), vel(_vel) {}
};

constexpr char WALL = '#';
constexpr char EMPTY = '.';
constexpr char BOX = 'O';
constexpr char BOX_LEFT = '[';
constexpr char BOX_RIGHT = ']';
constexpr char ROBOT = '@';

Coord findRobot(const std::vector<std::string>& grid);

std::pair<std::vector<std::string>, std::vector<Coord>> parseInputFile() {
    std::ifstream input("../input/day15.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::string> grid;
    std::string buffer;
    while (std::getline(input, buffer)) {
        if (buffer.empty()) {
            break;
        }
        grid.emplace_back(buffer);
    }

    std::vector<Coord> moves;
    while (std::getline(input, buffer)) {
        for (char c : buffer) {
            if (c == 'v') {
                moves.emplace_back(0, 1);
            } else if (c == '^') {
                moves.emplace_back(0, -1);
            } else if (c == '<') {
                moves.emplace_back(-1, 0);
            } else if (c == '>') {
                moves.emplace_back(1, 0);
            }
        }
    }

    return {grid, moves};
}

std::pair<std::vector<std::string>, std::vector<Coord>> parseInputFile2() {
    std::ifstream input("../input/day15.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::string> grid;
    std::string buffer;
    while (std::getline(input, buffer)) {
        if (buffer.empty()) {
            break;
        }
        std::string row;
        for (const char c : buffer) {
            if (c == BOX) {
                row += '[';
                row += ']';
            } else if (c == ROBOT) {
                row += '@';
                row += '.';
            } else {
                row += c;
                row += c;
            }
        }
        grid.emplace_back(row);
    }

    std::vector<Coord> moves;
    while (std::getline(input, buffer)) {
        for (char c : buffer) {
            if (c == 'v') {
                moves.emplace_back(0, 1);
            } else if (c == '^') {
                moves.emplace_back(0, -1);
            } else if (c == '<') {
                moves.emplace_back(-1, 0);
            } else if (c == '>') {
                moves.emplace_back(1, 0);
            }
        }
    }

    return {grid, moves};
}

bool doMove(std::vector<std::string>& grid, const Coord& move, const Coord& robot);

void printGrid(const std::vector<std::string>& grid) {
    std::cout << '\n';
    for (const auto& row : grid) {
        std::cout << row << '\n';
    }
    std::cout << '\n' << std::flush;
}

auto part1() {
    std::vector<std::string> grid;
    std::vector<Coord> moves;
    std::tie(grid, moves) = parseInputFile();
    Coord robot = findRobot(grid);

    for (const auto& move : moves) {
        if (doMove(grid, move, robot)) {
            robot += move;
        }
    }

    size_t result = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == BOX) {
                result += 100 * y + x;
            }
        }
    }

    return result;
}

bool doMove(std::vector<std::string>& grid, const Coord& move, const Coord& robot) {
    auto newPos = robot + move;
    if (grid[newPos.y][newPos.x] == EMPTY) {
        grid[newPos.y][newPos.x] = grid[robot.y][robot.x];
        grid[robot.y][robot.x] = EMPTY;
        return true;
    }
    if (grid[newPos.y][newPos.x] == WALL) {
        return false;
    }
    if (grid[newPos.y][newPos.x] == BOX) {
        if (doMove(grid, move, newPos)) {
            grid[newPos.y][newPos.x] = grid[robot.y][robot.x];
            grid[robot.y][robot.x] = EMPTY;
            return true;
        }
    }
    return false;
}

void updateGrid(std::vector<std::string>& grid, const Coord& oldPos, const Coord& newPos) {
    grid[newPos.y][newPos.x] = grid[oldPos.y][oldPos.x];
    grid[oldPos.y][oldPos.x] = EMPTY;
}

bool doMove2(std::vector<std::string>& grid, const Coord& move, const Coord& robot, std::vector<Coord>& thingsMoved) {
    auto newPos = robot + move;

    if (grid[newPos.y][newPos.x] == WALL) {
        return false;
    }
    if (grid[newPos.y][newPos.x] == EMPTY) {
        if (std::find(thingsMoved.begin(), thingsMoved.end(), robot) == thingsMoved.end()) {
            thingsMoved.push_back(robot);
        }
        return true;
    }
    if (grid[newPos.y][newPos.x] == BOX_LEFT) {
        if (move.y != 0) {
            if (doMove2(grid, move, newPos, thingsMoved) &&
                doMove2(grid, move, {newPos.x + 1, newPos.y}, thingsMoved)) {
                if (std::find(thingsMoved.begin(), thingsMoved.end(), robot) == thingsMoved.end()) {
                    thingsMoved.push_back(robot);
                }
                return true;
            }
        } else if (doMove2(grid, move, newPos, thingsMoved)) {
            thingsMoved.push_back(robot);
            return true;
        }
    }
    if (grid[newPos.y][newPos.x] == BOX_RIGHT) {
        if (move.y != 0) {
            if (doMove2(grid, move, newPos, thingsMoved) &&
                doMove2(grid, move, {newPos.x - 1, newPos.y}, thingsMoved)) {
                if (std::find(thingsMoved.begin(), thingsMoved.end(), robot) == thingsMoved.end()) {
                    thingsMoved.push_back(robot);
                }
                return true;
            }
        } else if (doMove2(grid, move, newPos, thingsMoved)) {
            thingsMoved.push_back(robot);
            return true;
        }
    }
    return false;
}

Coord findRobot(const std::vector<std::string>& grid) {
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == ROBOT) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

auto part2() {
    std::vector<std::string> grid;
    std::vector<Coord> moves;
    std::tie(grid, moves) = parseInputFile2();
    Coord robot = findRobot(grid);

    for (const auto& move : moves) {
        std::vector<Coord> thingsMoved;
        if (doMove2(grid, move, robot, thingsMoved)) {
            for (auto& thing : thingsMoved) {
                auto newPos = thing + move;
                updateGrid(grid, thing, newPos);
            }
            robot += move;
        }
    }

    size_t result = 0;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == BOX_LEFT) {
                result += 100 * y + x;
            }
        }
    }

    return result;
}

#endif // DAY15_HPP
