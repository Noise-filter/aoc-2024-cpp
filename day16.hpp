
#ifndef DAY16_HPP
#define DAY16_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>
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
};

struct Robot {
    Coord pos;
    Coord vel;
    Robot(const Coord _pos, const Coord _vel) : pos(_pos), vel(_vel) {}
};

constexpr char WALL = '#';
constexpr char START = 'S';
constexpr char END = 'E';

std::vector<std::string> parseInputFile() {
    std::ifstream input("../input/day16.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::string> grid;
    std::string buffer;
    while (std::getline(input, buffer)) {
        grid.push_back(buffer);
    }

    return grid;
}

std::pair<Coord, Coord> findStartAndEnd(const std::vector<std::string>& grid) {
    Coord start{0, 0}, end{0, 0};
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            if (grid[y][x] == START) {
                start = {x, y};
            } else if (grid[y][x] == END) {
                end = {x, y};
            }
        }
    }
    return {start, end};
}

struct Node {
    Coord pos;
    Coord dir{0, 0};
    int f;
    Node(Coord _pos, int _f) : pos(_pos), f(_f) {}
    Node(Coord _pos, Coord _dir, int _f) : pos(_pos), dir(_dir), f(_f) {}
    Node(long long _x, long long _y, int _f) : pos({_x, _y}), f(_f) {}
};

auto compare = [](const Node& lhs, const Node& rhs) { return false; };
const std::array<Coord, 4> DIRECTIONS{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};

int heuristic(const Coord& current, const Coord& dir, const Coord& neighbor) {
    auto newPos = current + dir;
    if (newPos == neighbor) {
        return 1;
    }
    if (newPos.x == neighbor.x || newPos.y == neighbor.y) {
        return 2001;
    }
    return 1001;
}

int heuristic(const Coord& current, const Coord& neighbor) {
    return std::pow(neighbor.x + current.x, 2) + std::pow(neighbor.y + current.y, 2);
}

std::vector<Node> getNeighbors(const std::vector<std::string>& grid, const Coord& current) {
    std::vector<Node> neighbors;

    for (const auto& dir : DIRECTIONS) {
        auto pos = dir + current;
        if (grid[pos.y][pos.x] != WALL) {
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

std::vector<Coord> findPath(const std::vector<std::string>& grid, const Coord& start, const Coord& end,
                            const Coord& direction) {
    std::priority_queue<Node, std::vector<Node>, decltype(compare)> openSet(compare);
    std::vector<int> gScore(grid.size() * grid[0].size(), (1 << 31) - 1);
    std::vector<Coord> cameFrom(grid.size() * grid[0].size(), {-1, -1});
    openSet.emplace(start, direction, heuristic(start, end));
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
            int tentativeG = gScore[currentIndex] + heuristic(current.pos, current.dir, neighbor.pos);
            int index = neighbor.pos.y * grid.size() + neighbor.pos.x;
            if (tentativeG < gScore[index]) {
                // Came from
                cameFrom[index] = current.pos;
                gScore[index] = tentativeG;
                neighbor.f = tentativeG + heuristic(current.pos, neighbor.pos);
                neighbor.dir = neighbor.pos - current.pos;
                openSet.push(neighbor);
            }
        }
    }

    return {};
}

auto part1() {
    std::vector<std::string> grid = parseInputFile();
    Coord start{0, 0};
    Coord end{0, 0};
    std::tie(start, end) = findStartAndEnd(grid);
    Coord direction{1, 0};

    std::vector<Coord> path = findPath(grid, start, end, direction);

    size_t score = 0;
    auto lastStep = start;
    for (const auto& step : path) {
        if (step == lastStep) {
            continue;
        }
        auto newDirection = step - lastStep;
        if (direction == newDirection) {
            ++score;
        } else {
            score += 1001;
        }
        grid[step.y][step.x] = '*';
        lastStep = step;
        direction = newDirection;
    }

    for (auto& row : grid) {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;

    return score;
}

struct TreeNode {
    Coord pos;
    bool turn = false;
};

bool floodFill(std::vector<std::string>& grid, std::set<int>& checked, const Coord& pos, const Coord& endPos,
               const Coord& dir) {
    if (grid[pos.y][pos.x] == WALL) {
        return false;
    }
    if (pos == endPos) {
        return true;
    }
    auto newPos = pos + dir;
    grid[pos.y][pos.x] = '*';

    std::bitset<3> results;
    results.set(0, floodFill(grid, checked, pos + dir, endPos, dir));
    if (dir.x == 0) {
        results.set(1, floodFill(grid, checked, pos + Coord{1, 0}, endPos, {1, 0}));
        results.set(2, floodFill(grid, checked, pos + Coord{-1, 0}, endPos, {-1, 0}));
    } else {
        results.set(1, floodFill(grid, checked, pos + Coord{0, 1}, endPos, {0, 1}));
        results.set(2, floodFill(grid, checked, pos + Coord{0, -1}, endPos, {0, -1}));
    }
    if (results.none()) {
        return false;
    }
    return true;
}

auto part2() {
    std::vector<std::string> grid = parseInputFile();
    Coord start{0, 0};
    Coord end{0, 0};
    std::tie(start, end) = findStartAndEnd(grid);
    Coord direction{1, 0};

    std::set<int> checked;
    floodFill(grid, checked, start, end, direction);

    return 0;
}

#endif // DAY16_HPP
