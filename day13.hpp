#ifndef DAY13_HPP
#define DAY13_HPP

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
    friend bool operator==(const Coord& lhs, const Coord& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
};

struct Machine {
    Coord a;
    Coord b;
    Coord goal;
    Machine(Coord _a, Coord _b, Coord _goal) : a(_a), b(_b), goal(_goal) {}
};

std::vector<Machine> parseInputFile() {
    std::ifstream input("../input/day13.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<Machine> machines;
    std::string buffer;
    while (std::getline(input, buffer)) {
        Coord a{stoi(buffer.substr(12, 2)), stoi(buffer.substr(18, 2))};

        // Button B
        std::getline(input, buffer);
        Coord b{stoi(buffer.substr(12, 2)), stoi(buffer.substr(18, 2))};

        // Prize
        std::getline(input, buffer);
        const auto index = buffer.find(',');
        Coord prize{stoi(buffer.substr(9, index - 9)), stoi(buffer.substr(index + 4))};

        machines.emplace_back(a, b, prize);
        // Empty line
        std::getline(input, buffer);
    }

    return machines;
}

auto part1() {
    std::vector<Machine> data = parseInputFile();

    size_t result = 0;
    for (auto& machine : data) {
        auto a = (machine.goal.x * machine.b.y - machine.goal.y * machine.b.x) /
                 (machine.a.x * machine.b.y - machine.a.y * machine.b.x);
        auto b = (machine.goal.y * machine.a.x - machine.goal.x * machine.a.y) /
                 (machine.a.x * machine.b.y - machine.a.y * machine.b.x);
        if (machine.a * a + machine.b * b == machine.goal) {
            result += a * 3 + b;
        }
    }

    return result;
}

auto part2() {
    std::vector<Machine> data = parseInputFile();

    size_t result = 0;
    for (auto& machine : data) {
        machine.goal.x += 10000000000000;
        machine.goal.y += 10000000000000;

        auto a = (machine.goal.x * machine.b.y - machine.goal.y * machine.b.x) /
                 (machine.a.x * machine.b.y - machine.a.y * machine.b.x);
        auto b = (machine.goal.y * machine.a.x - machine.goal.x * machine.a.y) /
                 (machine.a.x * machine.b.y - machine.a.y * machine.b.x);

        if (machine.a * a + machine.b * b == machine.goal) {
            result += (a * 3) + b;
        }
    }
    return result;
}

#endif // DAY13_HPP
