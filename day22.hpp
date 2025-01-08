#ifndef DAY22_HPP
#define DAY22_HPP

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>

std::vector<size_t> parseInputFile() {
    std::ifstream input("../input/day22.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }
    std::vector<size_t> numbers;
    size_t number;
    while (input >> number) {
        numbers.push_back(number);
        input.ignore();
    }

    return numbers;
}

size_t generateNextSecretNumber(size_t num) {
    num = num ^ (num * 64);
    num = num % 16777216;
    num = num ^ (num / 32);
    num = num % 16777216;
    num = num ^ (num * 2048);
    num = num % 16777216;

    return num;
}

auto part1() {
    std::vector<size_t> buyers = parseInputFile();

    constexpr int NUM_OF_NUMBERS = 2000;
    size_t result = 0;
    for (auto& secretNumber : buyers) {
        for (int i = 0; i < NUM_OF_NUMBERS; ++i) {
            secretNumber = generateNextSecretNumber(secretNumber);
        }
        result += secretNumber;
    }

    return result;
}

auto part2() {
    std::vector<size_t> buyers = parseInputFile();

    constexpr int NUM_OF_NUMBERS = 2001;
    std::vector<std::array<std::pair<int, int>, NUM_OF_NUMBERS>> secretNumbers;
    for (auto& secretNumber : buyers) {
        secretNumbers.emplace_back();
        secretNumbers[secretNumbers.size() - 1][0] = {secretNumber, 0};
        for (int i = 1; i < NUM_OF_NUMBERS; ++i) {
            secretNumber = generateNextSecretNumber(secretNumber);
            int diff = (secretNumber % 10) - (secretNumbers[secretNumbers.size() - 1][i - 1].first % 10);
            secretNumbers[secretNumbers.size() - 1][i] = {secretNumber, diff};
        }
    }

    std::set<std::array<int, 4>> sequences;
    for (const auto& secretNumber : secretNumbers) {
        int max = -1000;
        for (int i = 4; i < secretNumber.size(); ++i) {
            max = std::max(max, secretNumber[i].first % 10);
        }

        for (int i = 4; i < secretNumber.size(); ++i) {
            if (secretNumber[i].first % 10 == max) {
                std::array<int, 4> sequence{secretNumber[i - 3].second, secretNumber[i - 2].second,
                                            secretNumber[i - 1].second, secretNumber[i].second};
                sequences.insert(sequence);
            }
        }
    }

    size_t maxResult = 0;
    for (const auto& sequence : sequences) {
        size_t result = 0;
        for (const auto& secretNumber : secretNumbers) {
            for (int i = 1; i < secretNumber.size() - 4; ++i) {
                if (secretNumber[i].second == sequence[0] && secretNumber[i + 1].second == sequence[1] &&
                    secretNumber[i + 2].second == sequence[2] && secretNumber[i + 3].second == sequence[3]) {
                    result += secretNumber[i + 3].first % 10;
                    break;
                }
            }
        }
        maxResult = std::max(result, maxResult);
    }

    return maxResult;
}

#endif // DAY22_HPP
