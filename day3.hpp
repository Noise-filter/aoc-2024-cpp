#ifndef AOC2024_DAY3_HPP
#define AOC2024_DAY3_HPP

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

std::string parseInputFile() {
    std::ifstream input("../input/day3.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::string str;
    input.seekg(0, std::ios::end);
    str.reserve(input.tellg());
    input.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    return str;
}

auto part1() {
    int result = 0;

    std::string content = parseInputFile();

    std::regex re("mul\\((\\d+),(\\d+)\\)");

    for (std::smatch sm; regex_search(content, sm, re);) {
        // std::cout << sm.str() << '\n';
        auto num1 = std::stoi(sm[1].str());
        auto num2 = std::stoi(sm[2].str());
        result += (num1 * num2);
        content = sm.suffix();
    }

    return result;
}

auto part2() {
    int result = 0;

    std::string content = parseInputFile();

    bool enabled = true;
    std::regex re("don't\\(\\)|do\\(\\)");

    int count = 1;
    for (std::smatch sm; regex_search(content, sm, re);) {
        auto s = sm.str();
        std::cout << s << '\n';

        if (enabled) {
            std::regex re2("mul\\((\\d+),(\\d+)\\)");
            std::string pre = sm.prefix().str();
            for (std::smatch innerMatch; regex_search(pre, innerMatch, re2);) {
                auto num1 = std::stoi(innerMatch[1].str());
                auto num2 = std::stoi(innerMatch[2].str());
                result += (num1 * num2);
                pre = innerMatch.suffix();
            }
        }
        if (s == "don't()") {
            enabled = false;
        } else if (s == "do()") {
            enabled = true;
        }
        content = sm.suffix();
        ++count;
    }

    if (enabled) {
        std::regex re2("mul\\((\\d+),(\\d+)\\)");
        for (std::smatch innerMatch; regex_search(content, innerMatch, re2);) {
            auto num1 = std::stoi(innerMatch[1].str());
            auto num2 = std::stoi(innerMatch[2].str());
            result += (num1 * num2);
            content = innerMatch.suffix();
        }
    }

    return result;
}

#endif // AOC2024_DAY3_HPP
