#ifndef AOC2024_DAY9_HPP
#define AOC2024_DAY9_HPP

#include <algorithm>
#include <fstream>
#include <vector>

struct EmptySpace {
    int index;
    int length;

    EmptySpace(int index, int length) : index(index), length(length) {}
};

std::vector<EmptySpace> extractEmptyPlaces(const std::vector<int>& data);
long long int calculateChecksum(const std::vector<int>& data);

std::vector<int> parseInputFile() {
    std::ifstream input("../input/day9.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }

    std::vector<int> result;
    std::string line;
    std::getline(input, line);

    bool alternate = true;
    int id = 0;
    for (int i = 0; i < line.size(); i++) {
        int value = static_cast<int>(line[i] - '0');
        if (alternate) {
            result.insert(result.end(), value, id);
            id++;
        } else {
            result.insert(result.end(), value, -1);
        }
        alternate = !alternate;
    }

    return result;
}

auto part1() {
    std::vector<int> data = parseInputFile();

    auto emptyPlace = std::find(begin(data), end(data), -1);
    auto number = std::find_if(rbegin(data), rend(data), [](auto v) { return v != -1; });
    do {
        std::swap(*emptyPlace, *number);

        number = std::find_if(number, rend(data), [](auto v) { return v != -1; });
        emptyPlace = std::find(emptyPlace, end(data), -1);
    } while (emptyPlace < number.base());

    data.erase(emptyPlace, end(data));

    return calculateChecksum(data);
}

auto part2() {
    std::vector<int> data = parseInputFile();

    auto emptyPlaces = extractEmptyPlaces(data);

    auto startNumber = std::find_if(rbegin(data), rend(data), [](auto v) { return v != -1; });
    auto endNumber = std::find_if(startNumber, rend(data), [&](auto v) { return v != *startNumber; });

    while (endNumber < rend(data)) {
        auto numberLength = std::distance(startNumber, endNumber);
        auto startIndexOfNumber = std::distance(begin(data), endNumber.base());

        auto emptyPlaceIt = std::find_if(begin(emptyPlaces), end(emptyPlaces), [&](const EmptySpace& space) {
            return space.length >= numberLength && space.index < startIndexOfNumber;
        });

        if (emptyPlaceIt != end(emptyPlaces)) {
            for (int i = 0; i < numberLength; i++) {
                std::swap(data[emptyPlaceIt->index + i], *startNumber);
                startNumber = std::next(startNumber);
            }
            emptyPlaceIt->index = emptyPlaceIt->index + numberLength;
            emptyPlaceIt->length = emptyPlaceIt->length - numberLength;
        }

        startNumber = std::find_if(endNumber, rend(data), [](auto v) { return v != -1; });
        endNumber = std::find_if(startNumber, rend(data), [&](auto v) { return v != *startNumber; });
    }

    return calculateChecksum(data);
}

std::vector<EmptySpace> extractEmptyPlaces(const std::vector<int>& data) {
    int emptyPlaceStartIndex = -1;
    std::vector<EmptySpace> emptyPlaces;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] == -1 && emptyPlaceStartIndex == -1) {
            emptyPlaceStartIndex = i;
        } else if (data[i] != -1 && emptyPlaceStartIndex != -1) {
            emptyPlaces.emplace_back(emptyPlaceStartIndex, i - emptyPlaceStartIndex);
            emptyPlaceStartIndex = -1;
        }
    }
    return emptyPlaces;
}

long long int calculateChecksum(const std::vector<int>& data) {
    long long result = 0;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] != -1) {
            result += data[i] * i;
        }
    }
    return result;
}

#endif // AOC2024_DAY9_HPP
