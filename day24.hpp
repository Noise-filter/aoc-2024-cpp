
#ifndef DAY24_HPP
#define DAY24_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

struct Operation {
    std::string input1;
    std::string input2;
    std::string operation;
    std::string output;
    Operation(std::string input1, std::string input2, std::string operation, std::string output)
        : input1(std::move(input1)), input2(std::move(input2)), operation(std::move(operation)),
          output(std::move(output)) {}
};

std::vector<std::string> split(const std::string& str, char delim) {
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> out;

    while (std::getline(ss, item, delim)) {
        /*ss.ignore(1);
        if (item[0] == ' ') {
            item.erase(item.begin(), item.begin() + 1);
        }*/
        out.push_back(item);
    }
    return out;
}

std::pair<std::map<std::string, bool>, std::vector<Operation>> parseInputFile() {
    std::ifstream input("../input/day24.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::map<std::string, bool> inputMap;
    std::string buffer;
    while (std::getline(input, buffer)) {
        if (buffer.empty()) {
            break;
        }

        auto tokens = split(buffer, ' ');

        bool value = tokens[1][0] == '1';
        inputMap[tokens[0].substr(0, tokens[0].size() - 1)] = value;
    }

    std::vector<Operation> operations;
    while (std::getline(input, buffer)) {
        auto tokens = split(buffer, ' ');
        operations.emplace_back(tokens[0], tokens[2], tokens[1], tokens[4]);
    }

    return {inputMap, operations};
}

bool getValue(std::map<std::string, bool> inputMap, std::string& value) {
    auto it = inputMap.find(value);
    if (it != inputMap.end()) {
        return it->second;
    }
}

auto part1() {
    std::map<std::string, bool> inputMap;
    std::vector<Operation> operations;
    std::tie(inputMap, operations) = parseInputFile();

    bool allDone;
    do {
        allDone = true;
        for (const auto& operation : operations) {
            auto it = inputMap.find(operation.input1);
            bool input1;
            if (it != inputMap.end()) {
                input1 = it->second;
            } else {
                allDone = false;
                continue;
            }

            it = inputMap.find(operation.input2);
            bool input2;
            if (it != inputMap.end()) {
                input2 = it->second;
            } else {
                allDone = false;
                continue;
            }

            if (operation.operation == "OR") {
                inputMap[operation.output] = input1 | input2;
            } else if (operation.operation == "XOR") {
                inputMap[operation.output] = input1 ^ input2;
            } else if (operation.operation == "AND") {
                inputMap[operation.output] = input1 & input2;
            }
        }
    } while (allDone == false);

    std::string binaryNumber;
    for (const auto& pair : inputMap) {
        if (pair.first[0] == 'z') {
            std::cout << pair.first << std::endl;
            binaryNumber += std::to_string(pair.second);
        }
    }
    std::reverse(binaryNumber.begin(), binaryNumber.end());
    std::cout << binaryNumber << std::endl;
    return std::stoll(binaryNumber, nullptr, 2);
}

auto part2() {
    std::map<std::string, bool> inputMap;
    std::vector<Operation> operations;
    std::tie(inputMap, operations) = parseInputFile();

    return 0;
}

#endif // DAY24_HPP
