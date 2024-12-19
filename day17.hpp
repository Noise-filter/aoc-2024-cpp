
#ifndef DAY17_HPP
#define DAY17_HPP

#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using namespace std::chrono_literals;

std::pair<std::vector<uint16_t>, std::array<long long, 3>> parseInputFile() {
    std::ifstream input("../input/day17.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::array<long long, 3> registers{};
    std::string buffer;
    for (int i = 0; i < 3; i++) {
        input >> buffer;
        input >> buffer;
        input >> registers[i];
    }

    std::getline(input, buffer);
    input >> buffer;
    std::vector<uint16_t> program;
    uint16_t number;
    while (input >> number) {
        program.push_back(number);
        input.ignore(1);
    }

    return {program, registers};
}

enum class OpCode {
    ADV = 0,
    BXL = 1,
    BST = 2,
    JNZ = 3,
    BXC = 4,
    OUT = 5,
    BDV = 6,
    CDV = 7,
};

long long getComboValue(const std::array<long long, 3>& array, const uint8_t value) {
    if (value > 3 && value < 7) {
        return array[value - 4];
    }
    return value;
}

std::string execute(const std::vector<uint16_t>& program, std::array<long long, 3> registers,
                    const std::string& programString = "") {
    std::string result;
    result.reserve(program.size());
    for (int pc = 0; pc < program.size(); pc += 2) {
        OpCode opCode = static_cast<OpCode>(program[pc]);
        const uint8_t value = program[pc + 1];

        switch (opCode) {
            case OpCode::ADV:
                registers[0] = registers[0] / std::pow(2, getComboValue(registers, value));
                break;
            case OpCode::BXL:
                registers[1] = registers[1] ^ value;
                break;
            case OpCode::BST:
                registers[1] = getComboValue(registers, value) % 8;
                break;
            case OpCode::JNZ:
                if (registers[0] != 0) {
                    pc = value - 2;
                }
                break;
            case OpCode::BXC:
                registers[1] = registers[1] ^ registers[2];
                break;
            case OpCode::OUT: {
                result += std::to_string(getComboValue(registers, value) % 8);
                if (programString.rfind(result, 0) != 0) {
                    return result;
                }
            } break;
            case OpCode::BDV:
                registers[1] = registers[0] / std::pow(2, getComboValue(registers, value));
                break;
            case OpCode::CDV:
                registers[2] = registers[0] / std::pow(2, getComboValue(registers, value));
                break;
            default:
                std::cerr << "Unknown opcode: " << static_cast<uint8_t>(opCode) << std::endl;
                return "Unknown opcode";
        }
    }
    return result;
}

std::string part1() {
    std::vector<uint16_t> program;
    std::array<long long, 3> registers{};
    std::tie(program, registers) = parseInputFile();

    return execute(program, registers);
}

std::string bruteforce(const std::vector<uint16_t>& program, std::array<long long, 3> registers,
                       std::string programString, int index, int numberOfThreads, bool& exit) {
    for (long long i = index + 205000000000;; i += numberOfThreads) {
        registers[0] = i;
        if (registers[0] < 0) {
            return "FAIL";
        }
        if (registers[0] % 1000000000 == 0) {
            std::cout << "Index: " << i << ' ' << registers[0] << '\n';
        }

        std::string result = execute(program, registers, programString);
        if (result.size() > 10) {
            std::cout << result << ' ' << registers[0] << ' ' << registers[0] % 8 << ' ' << result.size() << std::endl;
        }
        if (programString == result) {
            return std::to_string(i);
        }
        if (exit) {
            return "FAIL";
        }
    }
    return "FAIL";
}

std::string part2() {
    std::vector<uint16_t> program;
    std::array<long long, 3> registers{};
    std::tie(program, registers) = parseInputFile();

    std::string programString;
    for (auto& num : program) {
        programString += std::to_string(num);
    }

    constexpr int numberOfThreads = 16;
    bool exit = false;

    std::array<std::future<std::string>, numberOfThreads> futures;
    for (int i = 0; i < numberOfThreads; i++) {
        futures[i] = std::async(bruteforce, program, registers, programString, i, numberOfThreads, std::ref(exit));
    }

    bool someoneStillRunning = true;
    while (someoneStillRunning) {
        someoneStillRunning = false;
        for (auto& future : futures) {
            if (future.valid() && future.wait_for(1s) == std::future_status::ready) {
                someoneStillRunning = true;
                auto value = future.get();
                if (value != "FAIL") {
                    exit = true;
                    return value;
                }
            }
        }
    }

    return "FAIL";
}

#endif // DAY17_HPP
