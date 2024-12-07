#ifndef DAY7_HPP
#define DAY7_HPP

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

void split(const std::string& str, std::vector<long long>& out) {
    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, ' ')) {
        out.push_back(std::stoi(item));
    }
}

std::vector<std::vector<long long>> parseInputFile() {
    std::ifstream input("../input/day7.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<std::vector<long long>> result;
    long long value;
    while (input >> value) {
        input.ignore(2);
        std::string temp;
        std::getline(input, temp);
        std::vector<long long> line{value};
        split(temp, line);
        result.push_back(line);
    }

    return result;
}

typedef std::function<long long(long long, long long)> Operation;

struct Op {
    Operation operation;
    int value = 0;
    friend bool operator<(const Op& lhs, const Op& rhs) { return lhs.value < rhs.value; }
    friend bool operator==(const Op& lhs, const Op& rhs) { return lhs.value == rhs.value; }
    friend bool operator!=(const Op& lhs, const Op& rhs) { return !(lhs == rhs); }
};

const Op plusOp{std::plus<>{}, 0};
const Op multiOp{std::multiplies<>{}, 1};
const Op concatOp{[](long long v1, long long v2) {
                      if (v2 >= 100) {
                          v1 *= 1000;
                      } else if (v2 >= 10) {
                          v1 *= 100;
                      } else {
                          v1 *= 10;
                      }
                      return v1 + v2;
                  },
                  2};

long long calculateSum(const std::vector<long long>& line, const std::vector<Op>& operators) {
    long long sum = line.at(1);
    for (int i = 2; i < line.size(); i++) {
        sum = operators.at(i - 2).operation(sum, line.at(i));
    }
    return sum;
}

bool doAllPermutations(const std::vector<long long>& line, std::vector<Op>& operators) {
    do {
        auto sum = calculateSum(line, operators);
        if (sum == line.at(0)) {
            return true;
        }
    } while (std::next_permutation(operators.begin(), operators.end()));
    return false;
}

bool checkLinePart1(const std::vector<long long>& line) {
    std::vector<Op> operators{line.size() - 1};

    for (int i = 0; i < line.size(); i++) {
        for (int j = 0; j < line.size(); j++) {
            if (j + i == line.size() - 1) {
                std::fill_n(operators.begin(), i, plusOp);
                std::fill_n(operators.begin() + i, j, multiOp);

                if (doAllPermutations(line, operators)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool checkLinePart2(const std::vector<long long>& line) {
    std::vector<Op> operators{line.size() - 1};

    for (int i = 0; i < line.size(); i++) {
        for (int j = 0; j < line.size(); j++) {
            for (int k = 0; k < line.size(); k++) {
                if (k + j + i == line.size() - 1) {
                    std::fill_n(operators.begin(), i, plusOp);
                    std::fill_n(operators.begin() + i, j, multiOp);
                    std::fill_n(operators.begin() + i + j, k, concatOp);

                    if (doAllPermutations(line, operators)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

auto part1() {
    long long result = 0;

    std::vector<std::vector<long long>> data = parseInputFile();

    for (const auto& line : data) {
        if (checkLinePart1(line)) {
            result += line.at(0);
        }
    }

    return result;
}

auto part2() {
    long long result = 0;

    std::vector<std::vector<long long>> data = parseInputFile();

    for (const auto& line : data) {
        if (checkLinePart2(line)) {
            result += line.at(0);
        }
    }

    return result;
}

#endif // DAY7_HPP
