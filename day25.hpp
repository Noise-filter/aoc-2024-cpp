#ifndef DAY19_HPP
#define DAY19_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

constexpr int LOCK_SIZE = 5;
constexpr int NUM_OF_LINES = 7;
using Lock = std::array<int, LOCK_SIZE>;
using Key = std::array<int, LOCK_SIZE>;

std::array<int, LOCK_SIZE> countColumns(const std::array<std::string, NUM_OF_LINES>& lines) {
    std::array<int, LOCK_SIZE> columns{};
    for (int i = 0; i < LOCK_SIZE; ++i) {
        int count = 0;
        for (int j = 0; j < 7; ++j) {
            if (lines[j][i] == '#') {
                ++count;
            }
        }
        columns[i] = count;
    }
    return columns;
}

std::pair<std::vector<Lock>, std::vector<Key>> parseInputFile() {
    std::ifstream input("../input/day25.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<Lock> locks;
    std::vector<Key> keys;
    std::array<std::string, NUM_OF_LINES> lines;
    while (std::getline(input, lines[0])) {
        for (int i = 1; i < NUM_OF_LINES; ++i) {
            std::getline(input, lines[i]);
        }
        input.ignore();

        if (std::all_of(lines[0].begin(), lines[0].end(), [](const unsigned char c) { return c == '#'; })) {
            locks.emplace_back(countColumns(lines));
        } else if (std::all_of(lines[NUM_OF_LINES - 1].begin(), lines[NUM_OF_LINES - 1].end(),
                               [](const unsigned char c) { return c == '#'; })) {
            keys.emplace_back(countColumns(lines));
        }
    }

    return {locks, keys};
}

bool checkKey(const Lock& lock, const Key& key) {
    for (int i = 0; i < LOCK_SIZE; ++i) {
        if (lock[i] + key[i] > NUM_OF_LINES) {
            return false;
        }
    }
    return true;
}

auto part1() {
    std::vector<Lock> locks;
    std::vector<Key> keys;
    std::tie(locks, keys) = parseInputFile();

    size_t result = 0;
    for (const auto& lock : locks) {
        for (const auto& key : keys) {
            if (checkKey(lock, key)) {
                ++result;
            }
        }
    }

    return result;
}

auto part2() {
    std::vector<Lock> locks;
    std::vector<Key> keys;
    std::tie(locks, keys) = parseInputFile();

    return 0;
}

#endif // DAY19_HPP
