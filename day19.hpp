#ifndef DAY19_HPP
#define DAY19_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

void split(const std::string& str, std::vector<std::string>& out) {
    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ',')) {
        char c;
        ss.ignore(1);
        if (item[0] == ' ') {
            item.erase(item.begin(), item.begin() + 1);
        }
        out.push_back(item);
    }
}

std::pair<std::vector<std::string>, std::vector<std::string>> parseInputFile() {
    std::ifstream input("../input/day19.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::string> patterns;
    std::string line;
    std::getline(input, line);
    split(line, patterns);

    input.ignore();
    std::vector<std::string> designs;
    while (std::getline(input, line)) {
        designs.push_back(line);
    }

    return {patterns, designs};
}

auto part1() {
    std::vector<std::string> patterns;
    std::vector<std::string> designs;
    std::tie(patterns, designs) = parseInputFile();

    

    return 0;
}

auto part2() {
    std::vector<std::string> patterns;
    std::vector<std::string> designs;
    std::tie(patterns, designs) = parseInputFile();

    return 0;
}

#endif // DAY19_HPP
