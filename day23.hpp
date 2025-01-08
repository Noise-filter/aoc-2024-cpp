#ifndef DAY23_HPP
#define DAY23_HPP

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

std::vector<std::pair<std::string, std::string>> parseInputFile() {
    std::ifstream input("../input/day23.txt");
    if (!input) {
        throw "Couldn't parse input file";
    }
    std::vector<std::pair<std::string, std::string>> pairs;
    char asd[3]{0};
    char asd2[3]{0};
    while (input.read(asd, 2)) {
        input.ignore(1);
        input.read(asd2, 2);
        input.ignore();
        pairs.emplace_back(asd, asd2);
    }

    return pairs;
}

auto part1() {
    std::vector<std::pair<std::string, std::string>> pairs = parseInputFile();

    std::map<std::string, std::vector<std::string>> map;
    for (const auto& pair : pairs) {
        auto it = map.find(pair.first);
        if (it == map.end()) {
            map[pair.first] = std::vector<std::string>{pair.second};
        } else {
            it->second.push_back(pair.second);
        }

        it = map.find(pair.second);
        if (it == map.end()) {
            map[pair.second] = std::vector<std::string>{pair.first};
        } else {
            it->second.push_back(pair.first);
        }
    }

    std::set<std::array<std::string, 3>> groups;
    for (auto& pair : map) {
        if (pair.first[0] == 't') {
            for (int i = 0; i < pair.second.size(); i++) {
                for (int j = i + 1; j < pair.second.size(); j++) {
                    auto found = std::find(map[pair.second[i]].begin(), map[pair.second[i]].end(), pair.second[j]);
                    if (found != map[pair.second[i]].end()) {
                        std::array<std::string, 3> a = {pair.first, pair.second[i], pair.second[j]};
                        std::sort(a.begin(), a.end());
                        groups.insert(std::move(a));
                    }
                }
            }
        }
    }

    return groups.size();
}

auto part2() {
    std::vector<std::pair<std::string, std::string>> pairs = parseInputFile();

    std::map<std::string, std::vector<std::string>> map;
    for (const auto& pair : pairs) {
        auto it = map.find(pair.first);
        if (it == map.end()) {
            map[pair.first] = std::vector<std::string>{pair.second};
        } else {
            it->second.push_back(pair.second);
        }

        it = map.find(pair.second);
        if (it == map.end()) {
            map[pair.second] = std::vector<std::string>{pair.first};
        } else {
            it->second.push_back(pair.first);
        }
    }

    std::vector<std::string> lanParty;
    for (auto& pair : map) {
        if (pair.first[0] == 't') {
            for (int i = 0; i < pair.second.size(); i++) {
                std::vector<std::string> party;
                party.push_back(pair.first);
                party.push_back(pair.second[i]);
                for (int j = 0; j < pair.second.size(); j++) {
                    if (i != j) {
                        if (std::all_of(party.begin(), party.end(), [&](const std::string& a) {
                                return std::find(map[a].begin(), map[a].end(), pair.second[j]) != map[a].end();
                            })) {
                            party.push_back(pair.second[j]);
                        }
                    }
                }
                if (party.size() > lanParty.size()) {
                    lanParty = std::move(party);
                }
            }
        }
    }

    std::sort(lanParty.begin(), lanParty.end());
    std::string password;
    for (const auto& name : lanParty) {
        password += name;
        password += ',';
    }
    password = password.substr(0, password.size() - 1);

    return password;
}

#endif // DAY23_HPP
