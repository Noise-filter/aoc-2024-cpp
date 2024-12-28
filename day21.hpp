#ifndef DAY21_HPP
#define DAY21_HPP

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Coord {
    int x;
    int y;
    Coord() : x(0), y(0) {}
    Coord(int _x, int _y) : x(_x), y(_y) {}

    friend Coord operator%(const Coord& lhs, const Coord& rhs) { return {lhs.x % rhs.x, lhs.y % rhs.y}; }
    friend Coord operator/(const Coord& lhs, const Coord& rhs) { return {lhs.x / rhs.x, lhs.y / rhs.y}; }
    friend Coord operator*(const Coord& lhs, const int number) { return {lhs.x * number, lhs.y * number}; }
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

std::vector<std::string> parseInputFile() {
    std::ifstream input("../input/day21.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }
    std::vector<std::string> lines;
    lines.resize(5);
    int counter = 0;
    while (std::getline(input, lines[counter++])) {
    }

    return lines;
}

std::string directionalKeypadFunc(const std::string& line, const std::map<char, Coord>& keypad) {
    auto currentPos = keypad.at('A');
    std::string keyPresses;

    for (const auto& c : line) {
        auto pos = keypad.at(c);
        auto diff = pos - currentPos;
        currentPos = pos;
        if (diff.x > 0) {
            keyPresses.insert(keyPresses.end(), diff.x, '>');
        }
        if (diff.y > 0) {
            keyPresses.insert(keyPresses.end(), diff.y, 'v');
        }
        if (diff.x < 0) {
            keyPresses.insert(keyPresses.end(), diff.x * -1, '<');
        }
        if (diff.y < 0) {
            keyPresses.insert(keyPresses.end(), diff.y * -1, '^');
        }
        keyPresses.push_back('A');
    }

    return keyPresses;
}

std::string numericKeypadFunc(const std::string& line, const std::map<char, Coord>& keypad) {
    auto currentPos = keypad.at('A');
    std::string keyPresses;

    for (const auto& c : line) {
        auto pos = keypad.at(c);
        auto diff = pos - currentPos;
        currentPos = pos;
        if (diff.x > 0) {
            keyPresses.insert(keyPresses.end(), diff.x, '>');
        }
        if (diff.y < 0) {
            keyPresses.insert(keyPresses.end(), diff.y * -1, '^');
        }
        if (diff.y > 0) {
            keyPresses.insert(keyPresses.end(), diff.y, 'v');
        }
        if (diff.x < 0) {
            keyPresses.insert(keyPresses.end(), diff.x * -1, '<');
        }
        keyPresses.push_back('A');
    }

    return keyPresses;
}

auto part1() {
    std::vector<std::string> lines = parseInputFile();

    std::map<char, Coord> numericKeypad;
    numericKeypad['7'] = Coord(0, 0);
    numericKeypad['8'] = Coord(1, 0);
    numericKeypad['9'] = Coord(2, 0);
    numericKeypad['4'] = Coord(0, 1);
    numericKeypad['5'] = Coord(1, 1);
    numericKeypad['6'] = Coord(2, 1);
    numericKeypad['1'] = Coord(0, 2);
    numericKeypad['2'] = Coord(1, 2);
    numericKeypad['3'] = Coord(2, 2);
    numericKeypad['0'] = Coord(1, 3);
    numericKeypad['A'] = Coord(2, 3);

    std::map<char, Coord> directionalKeypad;
    directionalKeypad['^'] = Coord(1, 0);
    directionalKeypad['A'] = Coord(2, 0);
    directionalKeypad['<'] = Coord(0, 1);
    directionalKeypad['v'] = Coord(1, 1);
    directionalKeypad['>'] = Coord(2, 1);

    size_t result = 0;
    for (const auto& line : lines) {
        auto buttonPresses = numericKeypadFunc(line, numericKeypad);
        std::cout << buttonPresses << std::endl;
        buttonPresses = directionalKeypadFunc(buttonPresses, directionalKeypad);
        std::cout << buttonPresses << std::endl;
        buttonPresses = directionalKeypadFunc(buttonPresses, directionalKeypad);
        std::cout << buttonPresses << std::endl;
        auto numeric = std::stoi(line.substr(0, line.size() - 1));
        std::cout << numeric << ' ' << buttonPresses.size() << std::endl;
        result += numeric * buttonPresses.size();
    }

    return result;
}

auto part2() {
    std::vector<std::string> lines = parseInputFile();

    return 0;
}

#endif // DAY21_HPP
