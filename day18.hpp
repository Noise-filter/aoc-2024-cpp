#ifndef AOC2024_DAY18_HPP
#define AOC2024_DAY18_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

constexpr char WALL = '#';

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

    friend std::ostream& operator<<(std::ostream& os, const Coord& lhs) {
        os << lhs.x << ',' << lhs.y;
        return os;
    }
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

constexpr int GRID_SIZE = 71;
using Grid = std::array<std::array<char, GRID_SIZE>, GRID_SIZE>;

void print(const Grid& grid) {
    for (const auto& row : grid) {
        for (const auto& p : row) {
            std::cout << p;
        }
        std::cout << '\n';
    }
}

struct Node {
    Coord pos;
    int f;
    Node(Coord _pos, int _f) : pos(_pos), f(_f) {}
    Node(long long _x, long long _y, int _f) : pos({_x, _y}), f(_f) {}
};

auto compare = [](const Node& lhs, const Node& rhs) { return lhs.f > rhs.f; };
const std::array<Coord, 4> DIRECTIONS{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

int heuristic(const Coord& current, const Coord& neighbor) {
    return 1 * (std::abs(current.x - neighbor.x) + std::abs(current.y - neighbor.y));
}

std::vector<Node> getNeighbors(const Grid& grid, const Coord& current) {
    std::vector<Node> neighbors;

    for (const auto& dir : DIRECTIONS) {
        auto pos = dir + current;
        if (grid[pos.y][pos.x] != WALL && pos.x >= 0 && pos.y >= 0 && pos.x < grid.size() && pos.y < grid.size()) {
            neighbors.emplace_back(pos, 0);
        }
    }

    return neighbors;
}

std::vector<Coord> reconstructPath(const std::vector<Coord>& cameFrom, const Node& current, int height) {
    std::vector<Coord> path;
    auto currentPos = current.pos;
    int index = currentPos.y * height + currentPos.x;
    while (index > 0 && cameFrom[index].x != -1) {
        path.emplace_back(currentPos);
        currentPos = cameFrom[index];
        index = currentPos.y * height + currentPos.x;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<Coord> findPath(const Grid& grid, const Coord& start, const Coord& end) {
    std::priority_queue<Node, std::vector<Node>, decltype(compare)> openSet(compare);
    std::vector<int> gScore(grid.size() * grid[0].size(), (1 << 31) - 1);
    std::vector<Coord> cameFrom(grid.size() * grid[0].size(), {-1, -1});
    openSet.emplace(start, heuristic(start, end));
    std::vector<Node> path;

    gScore[start.y * grid.size() + start.x] = 0;

    while (!openSet.empty()) {
        Node current = openSet.top();
        // std::cout << current.pos.x << ' ' << current.pos.y << '\n';
        int currentIndex = current.pos.y * grid.size() + current.pos.x;
        openSet.pop();
        if (current.pos == end) {
            return reconstructPath(cameFrom, current, grid.size());
        }

        for (auto& neighbor : getNeighbors(grid, current.pos)) {
            int tentativeG = gScore[currentIndex] + heuristic(current.pos, neighbor.pos);
            int index = neighbor.pos.y * grid.size() + neighbor.pos.x;
            if (tentativeG < gScore[index]) {
                // Came from
                cameFrom[index] = current.pos;
                gScore[index] = tentativeG;
                neighbor.f = tentativeG + heuristic(neighbor.pos, end);
                openSet.push(neighbor);
            }
        }
    }

    return {};
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

    auto path = findPath(grid, {0, 0}, {GRID_SIZE - 1, GRID_SIZE - 1});

    // for (auto& pos : path) {
    //     grid[pos.y][pos.x] = '*';
    // }
    //
    // std::cout << '\n';
    // print(grid);

    return path.size();
}

Coord part2() {
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

    auto path = findPath(grid, {0, 0}, {GRID_SIZE - 1, GRID_SIZE - 1});
    for (int i = 1023; i < data.size(); i++) {
        grid[data[i].y][data[i].x] = '#';
        for (auto& p : path) {
            if (p.x == data[i].x && p.y == data[i].y) {
                path = findPath(grid, {0, 0}, {GRID_SIZE - 1, GRID_SIZE - 1});
                if (path.empty()) {
                    return data[i];
                }
                break;
            }
        }
    }

    return {0, 0};
}

#endif // AOC2024_DAY18_HPP
