
#ifndef DAY24_HPP
#define DAY24_HPP

#include <algorithm>
#include <array>
#include <bitset>
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

    friend std::ostream& operator<<(std::ostream& os, const Operation& op) {
        os << op.input1 << ' ' << op.operation << ' ' << op.input2 << " -> " << op.output;
        return os;
    }
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

std::pair<std::map<std::string, bool>, std::vector<Operation>> parseInputFile(const std::string& filename) {
    std::ifstream input("../input/" + filename);
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

std::string createBinaryNumber(std::map<std::string, bool> inputMap, char firstChar) {
    std::string binaryNumber;
    for (const auto& pair : inputMap) {
        if (pair.first[0] == firstChar) {
            binaryNumber += std::to_string(pair.second);
        }
    }
    std::reverse(binaryNumber.begin(), binaryNumber.end());
    return binaryNumber;
}

void executeOperations(std::map<std::string, bool>& inputMap, std::vector<Operation> operations) {
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
}

auto part1() {
    std::map<std::string, bool> inputMap;
    std::vector<Operation> operations;
    std::tie(inputMap, operations) = parseInputFile("day24.txt");

    executeOperations(inputMap, operations);
    std::string zBinaryNumber = createBinaryNumber(inputMap, 'z');
    return std::stoll(zBinaryNumber, nullptr, 2);
}

std::string createStringNumber(const int i) {
    std::string stringNumber;
    if (i < 10) {
        stringNumber += '0';
    }
    stringNumber += std::to_string(i);
    return stringNumber;
}

struct AdditionOperations {
    Operation* andOperation{nullptr};
    Operation* xorOperation{nullptr};
    Operation* overflowAndOperation{nullptr};
    Operation* resultXorOperation{nullptr};
    Operation* overflowOrOperation{nullptr};

    friend std::ostream& operator<<(std::ostream& os, const AdditionOperations& op) {
        if (op.andOperation) {
            os << *op.andOperation << '\n';
        }
        if (op.xorOperation) {
            os << *op.xorOperation << '\n';
        }
        if (op.overflowAndOperation) {
            os << *op.overflowAndOperation << '\n';
        }
        if (op.resultXorOperation) {
            os << *op.resultXorOperation << '\n';
        }
        if (op.overflowOrOperation) {
            os << *op.overflowOrOperation << '\n';
        }
        return os;
    }
};

bool hasInputs(const Operation& operation, const std::string& input1, const std::string& input2) {
    return (operation.input1 == input1 || operation.input1 == input2) &&
           (operation.input2 == input1 || operation.input2 == input2);
}

auto part2() {
    std::map<std::string, bool> inputMap;
    std::vector<Operation> operations;
    std::tie(inputMap, operations) = parseInputFile("day24_part2.txt");

    executeOperations(inputMap, operations);
    std::string xBinaryNumber = createBinaryNumber(inputMap, 'x');
    std::string yBinaryNumber = createBinaryNumber(inputMap, 'y');
    std::string zBinaryNumber = createBinaryNumber(inputMap, 'z');

    long long xNumber = std::stoll(xBinaryNumber, nullptr, 2);
    long long yNumber = std::stoll(yBinaryNumber, nullptr, 2);
    long long zNumber = std::stoll(zBinaryNumber, nullptr, 2);

    auto expectedNumber = xNumber + yNumber;
    auto expectedBinary = std::bitset<46>(expectedNumber).to_string();
    auto diffBinary = std::bitset<46>(expectedNumber ^ zNumber).to_string();

    std::cout << std::endl;
    std::cout << xBinaryNumber << ' ' << xNumber << std::endl;
    std::cout << yBinaryNumber << ' ' << yNumber << std::endl;
    std::cout << expectedBinary << ' ' << expectedNumber << std::endl;
    std::cout << diffBinary << std::endl;
    std::cout << zBinaryNumber << ' ' << zNumber << std::endl;

    for (int i = diffBinary.size() - 1; i >= 0; --i) {
        if (diffBinary[i] == '1') {
            std::cout << i << ':' << diffBinary[i] << std::endl;
        }
    }

    std::array<AdditionOperations, 45> additionOperations;

    for (auto& operation : operations) {
        if (operation.operation == "AND" && hasInputs(operation, "x00", "y00")) {
            additionOperations[0].andOperation = additionOperations[0].overflowOrOperation = &operation;
        }
        if (operation.operation == "XOR" && hasInputs(operation, "x00", "y00")) {
            additionOperations[0].resultXorOperation = &operation;
        }
    }
    for (int i = 1; i < 45; ++i) {
        std::string stringNumber = createStringNumber(i);

        for (auto& operation : operations) {
            if (operation.operation == "AND" && hasInputs(operation, "x" + stringNumber, "y" + stringNumber)) {
                additionOperations[i].andOperation = &operation;
            }
            if (operation.operation == "XOR" && hasInputs(operation, "x" + stringNumber, "y" + stringNumber)) {
                additionOperations[i].xorOperation = &operation;
            }
        }

        for (auto& operation : operations) {
            if (operation.operation == "AND" && hasInputs(operation, additionOperations[i].xorOperation->output,
                                                          additionOperations[i - 1].overflowOrOperation->output)) {
                additionOperations[i].overflowAndOperation = &operation;
            }
            if (operation.operation == "XOR" && hasInputs(operation, additionOperations[i].xorOperation->output,
                                                          additionOperations[i - 1].overflowOrOperation->output)) {
                additionOperations[i].resultXorOperation = &operation;
            }
        }

        for (auto& operation : operations) {
            if (operation.operation == "OR" && hasInputs(operation, additionOperations[i].andOperation->output,
                                                         additionOperations[i].overflowAndOperation->output)) {
                additionOperations[i].overflowOrOperation = &operation;
            }
        }

        std::cout << additionOperations[i] << std::endl;
    }

    xBinaryNumber = createBinaryNumber(inputMap, 'x');
    yBinaryNumber = createBinaryNumber(inputMap, 'y');
    zBinaryNumber = createBinaryNumber(inputMap, 'z');

    xNumber = std::stoll(xBinaryNumber, nullptr, 2);
    yNumber = std::stoll(yBinaryNumber, nullptr, 2);
    zNumber = std::stoll(zBinaryNumber, nullptr, 2);

    expectedNumber = xNumber + yNumber;
    expectedBinary = std::bitset<46>(expectedNumber).to_string();
    diffBinary = std::bitset<46>(expectedNumber ^ zNumber).to_string();

    std::cout << std::endl;
    std::cout << xBinaryNumber << ' ' << xNumber << std::endl;
    std::cout << yBinaryNumber << ' ' << yNumber << std::endl;
    std::cout << expectedBinary << ' ' << expectedNumber << std::endl;
    std::cout << diffBinary << std::endl;
    std::cout << zBinaryNumber << ' ' << zNumber << std::endl;

    for (int i = diffBinary.size() - 1; i >= 0; --i) {
        if (diffBinary[i] == '1') {
            std::cout << i << ':' << diffBinary[i] << std::endl;
        }
    }

    /*
     * The result is hardcoded. The input for this day is also changed to the correct result.
     */
    return "bmn,jss,mvb,rds,wss,z08,z18,z23";
}

#endif // DAY24_HPP
