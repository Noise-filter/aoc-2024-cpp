#include "day13.hpp"

#include <chrono>
#include <iostream>

template <typename Function>
auto benchmark(Function f) {
    auto start = std::chrono::steady_clock::now();
    auto result = f();
    auto stop = std::chrono::steady_clock::now();

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << microseconds.count() << " us. Result: ";

    return result;
}

int main() {
    std::cout << "Part1: " << benchmark(part1) << '\n';
    std::cout << "Part2: " << benchmark(part2) << '\n';
    return 0;
}
