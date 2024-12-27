#ifndef DAY19_HPP
#define DAY19_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
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

bool startsWith(const std::string& design, const std::string& pattern) {
    if (pattern.size() > design.size()) {
        return false;
    }
    for (int i = 0; i < pattern.size(); i++) {
        if (design[i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

bool startsWith(const std::string& design, size_t index, const std::string& pattern) {
    if (index + pattern.size() > design.size()) {
        return false;
    }
    for (int i = 0; i < pattern.size(); i++) {
        if (design[i + index] != pattern[i]) {
            return false;
        }
    }
    return true;
}

bool tryDesign(const std::string& design, const std::map<char, std::vector<std::string>>& patternsMap, size_t index) {
    if (index == design.size()) {
        return true;
    }
    if (patternsMap.find(design[index]) != patternsMap.end()) {
        for (const auto& pattern : patternsMap.at(design[index])) {
            if (startsWith(design, index, pattern) && tryDesign(design, patternsMap, pattern.size() + index)) {
                return true;
            }
        }
    }
    return false;
}

std::map<char, std::vector<std::string>> generatePatternMap(const std::vector<std::string>& patterns) {
    std::map<char, std::vector<std::string>> patternsMap;
    for (const auto& pattern : patterns) {
        if (patternsMap.find(pattern[0]) == patternsMap.end()) {
            patternsMap[pattern[0]] = {pattern};
        } else {
            patternsMap[pattern[0]].push_back(pattern);
        }
    }
    return patternsMap;
}

auto part1() {
    std::vector<std::string> patterns;
    std::vector<std::string> designs;
    std::tie(patterns, designs) = parseInputFile();

    std::map<char, std::vector<std::string>> patternsMap = generatePatternMap(patterns);
    size_t result = 0;
    for (const auto& design : designs) {
        result += tryDesign(design, patternsMap, 0);
    }

    return result;
}

size_t tryAllDesigns(const std::string& design, const std::map<char, std::vector<std::string>>& patternsMap,
                     std::map<std::string, size_t>& cache) {
    if (design.empty()) {
        return 1;
    }

    if (cache.find(design) != cache.end()) {
        return cache[design];
    }

    size_t count = 0;
    if (patternsMap.find(design[0]) != patternsMap.end()) {
        for (const auto& pattern : patternsMap.at(design[0])) {
            if (startsWith(design, pattern)) {
                count += tryAllDesigns(design.substr(pattern.size()), patternsMap, cache);
            }
        }
    }

    cache[design] = count;
    return count;
}

auto part2() {
    std::vector<std::string> patterns;
    std::vector<std::string> designs;
    std::tie(patterns, designs) = parseInputFile();

    std::map<char, std::vector<std::string>> patternsMap = generatePatternMap(patterns);
    std::map<std::string, size_t> cache;
    size_t result = 0;
    for (const auto& design : designs) {
        result += tryAllDesigns(design, patternsMap, cache);
    }

    return result;
}

#endif // DAY19_HPP
