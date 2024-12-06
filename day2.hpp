#ifndef AOC2024_DAY2_HPP
#define AOC2024_DAY2_HPP

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <tuple>
#include <vector>

std::vector<std::vector<int>> parseInputFile() {
    std::ifstream input("../input/day2.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::vector<int>> numbers;
    std::string temp;
    while (std::getline(input, temp)) {
        std::istringstream buffer(temp);
        numbers.emplace_back(std::istream_iterator<int>(buffer), std::istream_iterator<int>());
    }

    return numbers;
}

void printRow(const std::vector<int>& row) {
    for (const int i : row) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

void printNumbers(const std::vector<std::vector<int>>& numbers) {
    for (const auto& row : numbers) {
        printRow(row);
    }
}

template <class BinaryPred>
bool checkRow(const std::vector<int>& row, BinaryPred p) {
    if (std::adjacent_find(begin(row), end(row), p) == end(row)) {
        return true;
    }

    auto copy = row;
    for (int i = 0; i < row.size(); i++) {
        copy = row;
        copy.erase(std::next(begin(copy), i));
        if (std::adjacent_find(begin(copy), end(copy), p) == end(copy)) {
            return true;
        }
    }
    return false;
}

auto part1() {
    int result = 0;

    std::vector<std::vector<int>> numbers = parseInputFile();

    for (const auto& row : numbers) {
        auto greater_equal = [](int a, int b) { return a >= b || std::abs(a - b) > 3; };
        auto less_equal = [](int a, int b) { return a <= b || std::abs(a - b) > 3; };
        if (std::adjacent_find(begin(row), end(row), greater_equal) == end(row) ||
            std::adjacent_find(begin(row), end(row), less_equal) == end(row)) {
            ++result;
        }
    }

    return result;
}

auto part2() {
    int result = 0;

    std::vector<std::vector<int>> numbers = parseInputFile();

    for (const auto& row : numbers) {
        auto greater_equal = [](int a, int b) { return a >= b || std::abs(a - b) > 3; };
        auto less_equal = [](int a, int b) { return a <= b || std::abs(a - b) > 3; };
        if (checkRow(row, greater_equal) || checkRow(row, less_equal)) {
            ++result;
        }
    }

    return result;
}

#endif // AOC2024_DAY2_HPP
