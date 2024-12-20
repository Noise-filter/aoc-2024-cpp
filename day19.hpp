#ifndef AOC2024_DAY19_HPP
#define AOC2024_DAY19_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>

constexpr char START = 'S';
constexpr char END = 'E';
constexpr char WALL = '#';
constexpr char EMPTY = '.';
constexpr int GRID_SIZE = 141;
using Grid = std::array<std::array<char, GRID_SIZE>, GRID_SIZE>;

struct Coord {
    long long x;
    long long y;
    Coord() : x(0), y(0) {}
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

Grid parseInputFile() {
    std::ifstream input("../input/day19.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    Grid grid;
    std::string line;
    int i = 0;
    while (std::getline(input, line)) {
        std::copy(line.begin(), line.end(), grid[i].data());
        i++;
    }

    return grid;
}

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
    while (index >= 0 && cameFrom[index].x != -1) {
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

std::pair<Coord, Coord> findStartAndEnd(const Grid& grid) {
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

auto part1() {
    Grid grid = parseInputFile();
    Coord start;
    Coord end;
    std::tie(start, end) = findStartAndEnd(grid);

    auto path = findPath(grid, start, end);
    path.insert(path.begin(), start);
    // for (auto& pos : path) {
    //     grid[pos.y][pos.x] = '*';
    // }
    // std::cout << '\n';
    // print(grid);

    size_t result = 0;
    std::array<Coord, 4> directions{{{0, 2}, {0, -2}, {2, 0}, {-2, 0}}};
    for (int i = 0; i < path.size(); i++) {
        for (const auto& dir : directions) {
            auto newPos = path[i] + dir;
            if (grid[newPos.y][newPos.x] == '.') {
                for (int j = i + 102; j < path.size(); j++) {
                    if (newPos == path[j]) {
                        auto save = j - i - 2;
                        if (save >= 100) {
                            result += 1;
                        }
                        break;
                    }
                }
            }
        }
    }

    // for (auto& pos : path) {
    //     grid[pos.y][pos.x] = '*';
    // }
    // std::cout << '\n';
    // print(grid);

    return result;
}

long long distance(const Coord& pos1, const Coord& pos2) {
    auto dist = pos1 - pos2;
    return std::abs(dist.x) + std::abs(dist.y);
}

auto part2() {
    Grid grid = parseInputFile();
    Coord start;
    Coord end;
    std::tie(start, end) = findStartAndEnd(grid);

    auto path = findPath(grid, start, end);
    path.insert(path.begin(), start);

    size_t result = 0;
    for (int i = 0; i < path.size(); i++) {
        for (int j = i + 1; j < path.size(); j++) {
            auto dist = distance(path[i], path[j]);
            if (dist <= 20) {
                auto save = j - i - dist;
                if (save >= 100) {
                    result += 1;
                }
            }
        }
    }

    return result;
}

#endif // AOC2024_DAY19_HPP
