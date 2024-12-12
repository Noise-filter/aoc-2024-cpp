#ifndef DAY11_HPP
#define DAY11_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

std::vector<long long> parseInputFile() {
    std::ifstream input("../input/day11.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<long long> result;
    int stone = 0;
    while (input >> stone) {
        result.push_back(stone);
    }

    return result;
}

inline long long countDigits(const size_t number) {
    return number > 0 ? static_cast<int>(log10(static_cast<double>(number)) + 1) : 1;
}

size_t blink(size_t stone, int blinks, std::vector<std::vector<size_t>>& cache) {
    if (blinks == 0) {
        return 1;
    }
    blinks -= 1;

    if (stone < cache.size() && cache[stone][blinks] != 0) {
        return cache[stone][blinks];
    }

    if (stone == 0) {
        size_t result = blink(1, blinks, cache);
        cache[stone][blinks] = result;
        return result;
    }

    long long count = countDigits(stone);
    if (count % 2 == 0) {
        long powNumber = lround(std::pow(10, count / 2));

        size_t result = blink(stone / powNumber, blinks, cache) + blink(stone % powNumber, blinks, cache);
        if (stone < cache.size()) {
            cache[stone][blinks] = result;
        }
        return result;
    }

    size_t result = blink(stone * 2024, blinks, cache);
    if (stone < cache.size()) {
        cache[stone][blinks] = result;
    }
    return result;
}

auto part1() {
    std::vector<long long> stones = parseInputFile();

    int blinks = 25;

    int cacheSize = 100;
    std::vector<std::vector<size_t>> cache(cacheSize, std::vector<size_t>(blinks, 0));

    size_t result = 0;
    for (auto stone : stones) {
        result += blink(stone, blinks, cache);
    }

    return result;
}

auto part2() {
    std::vector<long long> stones = parseInputFile();

    int blinks = 75;

    int cacheSize = 2000;
    std::vector<std::vector<size_t>> cache(cacheSize, std::vector<size_t>(blinks, 0));

    size_t result = 0;
    for (auto stone : stones) {
        result += blink(stone, blinks, cache);
    }
    return result;
}

#endif // DAY11_HPP
