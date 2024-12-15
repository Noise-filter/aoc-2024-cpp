#ifndef DAY13_HPP
#define DAY13_HPP

#include <algorithm>
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

    buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.cend());

    return machines;
}

constexpr long long gcd(const long long a, const long long b) {
    // Everything divides 0
    if (a == 0)
        return b;
    if (b == 0)
        return a;

    // Base case
    if (a == b)
        return a;

    // a is greater
    if (a > b)
        return gcd(a - b, b);
    return gcd(a, b - a);
}

auto part1() {
    std::vector<Machine> data = parseInputFile();

    size_t result = 0;
    for (const auto& machine : data) {
        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 100; y++) {
                if (machine.a * x + machine.b * y == machine.goal) {
                    result += x * 3 + y;
                    auto gcdResult1 = gcd(machine.a.x, machine.a.y);
                    auto gcdResult2 = gcd(machine.b.x, machine.b.y);
                    std::cout << gcdResult1 << ' ' << gcdResult2 << std::endl;

                }
            }
        }
    }

    return result;
}

auto part2() {
    std::vector<Machine> data = parseInputFile();
    for (auto& machine : data) {
        // machine.goal.x += 10000000000000;
        // machine.goal.y += 10000000000000;

        auto gcdResult1 = gcd(machine.a.x, machine.a.y);
        auto gcdResult2 = gcd(machine.b.x, machine.b.y);
        if (gcdResult1 != 1) {
            auto a = machine.a.x / gcdResult1;
            auto b = machine.a.y / gcdResult1;
        } else if (gcdResult2 != 1) {
        }
        std::cout << gcdResult1 << ' ' << gcdResult2 << std::endl;
    }

    return 0;
}

#endif // DAY13_HPP
