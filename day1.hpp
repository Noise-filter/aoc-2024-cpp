#ifndef AOC2024_DAY1_HPP
#define AOC2024_DAY1_HPP

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> parseInputFile() {
    std::ifstream input("../input/day1.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    int num1 = 0;
    int num2 = 0;
    std::vector<int> numbers1, numbers2;
    while (!input.eof()) {
        input >> num1 >> num2;
        numbers1.push_back(num1);
        numbers2.push_back(num2);
    }

    return {numbers1, numbers2};
}

auto part1() {
    int result = 0;

    std::vector<int> numbers1, numbers2;
    std::tie(numbers1, numbers2) = parseInputFile();

    std::sort(begin(numbers1), end(numbers1));
    std::sort(begin(numbers2), end(numbers2));

    for (int i = 0; i < numbers1.size(); i++) {
        result += std::abs(numbers1[i] - numbers2[i]);
    }

    return result;
}

auto part2() {
    int result = 0;

    std::vector<int> numbers1, numbers2;
    std::tie(numbers1, numbers2) = parseInputFile();

    for (const auto i : numbers1) {
        result += (i * std::count(begin(numbers2), end(numbers2), i));
    }

    return result;
}

#endif // AOC2024_DAY1_HPP
