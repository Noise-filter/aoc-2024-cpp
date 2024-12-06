#ifndef AOC2024_DAY5_HPP
#define AOC2024_DAY5_HPP

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

void insertOrPushBack(std::map<int, std::vector<int>>& rules, int value1, int value2) {
    auto it = rules.find(value2);
    if (it == rules.end()) {
        rules.insert({value2, {value1}});
    } else {
        it->second.push_back(value1);
    }
}

std::pair<std::map<int, std::vector<int>>, std::vector<std::vector<int>>> parseInputFile() {
    std::ifstream input("../input/day5.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> pages;
    std::string buffer;
    while (std::getline(input, buffer)) {
        auto pos = buffer.find('|');
        if (pos != std::string::npos) {
            int value1 = std::stoi(buffer.substr(0, pos));
            int value2 = std::stoi(buffer.substr(pos + 1));
            insertOrPushBack(rules, value1, value2);
        } else {
            size_t startPos = 0;
            std::vector<int> values;
            do {
                pos = buffer.find(',', startPos);
                int value = std::stoi(buffer.substr(startPos, pos - startPos));
                values.push_back(value);
                startPos = pos + 1;
            } while (pos != std::string::npos);
            pages.push_back(std::move(values));
        }
    }

    return {rules, pages};
}

bool checkList(const std::map<int, std::vector<int>>& rules, const std::vector<int>& list) {
    for (size_t i = 0; i < list.size(); i++) {
        auto it = rules.find(list[i]);
        if (it == rules.end()) {
            continue;
        }
        for (int j = i + 1; j < list.size(); j++) {
            for (int page : it->second) {
                if (page == list[j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool checkValue(const std::vector<int>& rulePages, std::vector<int>& list, int i) {
    for (int j = list.size() - 1; j > i; j--) {
        for (int page : rulePages) {
            if (page == list[j]) {
                std::swap(list[i], list[j]);
                return true;
            }
        }
    }
    return false;
}

bool reorderList(const std::map<int, std::vector<int>>& rules, std::vector<int>& list) {
    bool changed = false;
    for (size_t i = 0; i < list.size(); i++) {
        auto it = rules.find(list[i]);
        if (it == rules.end()) {
            continue;
        }
        if (checkValue(it->second, list, i)) {
            changed = true;
            i--;
        }
    }
    return changed;
}

auto part1() {
    int result = 0;

    // Values in the vector are not allowed to be behind the key in the pages vector.
    std::map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> pages;
    std::tie(rules, pages) = parseInputFile();

    for (const auto& list : pages) {
        if (checkList(rules, list)) {
            result += list[list.size() / 2];
        }
    }

    return result;
}

auto part2() {
    int result = 0;

    std::map<int, std::vector<int>> rules;
    std::vector<std::vector<int>> pages;
    std::tie(rules, pages) = parseInputFile();

    for (auto& list : pages) {
        if (reorderList(rules, list)) {
            result += list[list.size() / 2];
        }
    }

    return result;
}

#endif // AOC2024_DAY5_HPP
