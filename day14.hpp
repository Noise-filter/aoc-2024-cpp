#ifndef DAY14_HPP
#define DAY14_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
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
};

struct Robot {
    Coord pos;
    Coord vel;
    Robot(const Coord _pos, const Coord _vel) : pos(_pos), vel(_vel) {}
};

std::vector<Robot> parseInputFile() {
    std::ifstream input("../input/day14.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<Robot> robots;
    std::string buffer;
    while (std::getline(input, buffer)) {
        auto commaIndex = buffer.find(',');
        auto spaceIndex = buffer.find(' ');

        Coord pos{stoi(buffer.substr(2, commaIndex)), stoi(buffer.substr(commaIndex + 1, spaceIndex))};
        commaIndex = buffer.find(',', commaIndex + 1);
        Coord vel{stoi(buffer.substr(spaceIndex + 3, commaIndex)), stoi(buffer.substr(commaIndex + 1))};
        robots.emplace_back(pos, vel);
    }

    return robots;
}

constexpr int GRID_WIDTH = 101;
constexpr int GRID_HEIGHT = 103;
const Coord GRID_SIZE(GRID_WIDTH, GRID_HEIGHT);

auto part1() {
    std::vector<Robot> data = parseInputFile();

    constexpr int seconds = 100;
    for (int i = 0; i < seconds; i++) {
        for (auto& robot : data) {
            robot.pos += robot.vel;
            robot.pos %= GRID_SIZE;
            while (robot.pos.x < 0) {
                robot.pos.x += GRID_WIDTH;
            }
            while (robot.pos.y < 0) {
                robot.pos.y += GRID_HEIGHT;
            }
        }
    }

    long long upperLeft = 0;
    long long upperRight = 0;
    long long lowerLeft = 0;
    long long lowerRight = 0;
    for (auto& robot : data) {
        if (robot.pos.x < GRID_WIDTH / 2 && robot.pos.y < GRID_HEIGHT / 2) {
            upperLeft += 1;
        } else if (robot.pos.x < GRID_WIDTH / 2 && robot.pos.y > GRID_HEIGHT / 2) {
            lowerLeft += 1;
        } else if (robot.pos.x > GRID_WIDTH / 2 && robot.pos.y < GRID_HEIGHT / 2) {
            upperRight += 1;
        } else if (robot.pos.x > GRID_WIDTH / 2 && robot.pos.y > GRID_HEIGHT / 2) {
            lowerRight += 1;
        }
    }

    return upperLeft * upperRight * lowerLeft * lowerRight;
}

auto part2() {
    std::vector<Robot> data = parseInputFile();

    std::array<std::string, GRID_HEIGHT> grid;
    for (auto& row : grid) {
        row = std::string(GRID_WIDTH, ' ');
    }

    for (auto& robot : data) {
        grid[robot.pos.y][robot.pos.x] = 'x';
    }

    constexpr int seconds = 10000;
    for (int i = 0; i < seconds; i++) {
        for (auto& row : grid) {
            int count = 0;
            int maxCount = 0;
            for (char c : row) {
                if (c == 'x') {
                    count++;
                    maxCount = std::max(count, maxCount);
                } else {
                    count = 1;
                }
                if (maxCount > 14) {
                    return i;
                }
            }
        }
        for (auto& robot : data) {
            grid[robot.pos.y][robot.pos.x] = ' ';
            robot.pos += robot.vel;
            robot.pos %= GRID_SIZE;
            while (robot.pos.x < 0) {
                robot.pos.x += GRID_WIDTH;
            }
            while (robot.pos.y < 0) {
                robot.pos.y += GRID_HEIGHT;
            }
            grid[robot.pos.y][robot.pos.x] = 'x';
        }
    }

    return 0;
}

#endif // DAY14_HPP
