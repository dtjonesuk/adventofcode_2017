//
// Created by David on 01/12/2023.
//
#ifndef ADVENT_OF_CODE_2017_ADVENT_H
#define ADVENT_OF_CODE_2017_ADVENT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <numeric>
#include <ranges>
#include <regex>
#include <chrono>
#include <cassert>

/// Benchmarks a callable
template<typename T>
std::pair<double, T> eval(std::function<T(void)> func) {
    const auto t1 = std::chrono::high_resolution_clock::now();
    T result = func();
    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> seconds = t2 - t1;

    return std::make_pair(seconds.count(), result);
}

template<typename T>
void run(const std::string &day, const std::string &part, std::function<T(void)> func) {
    std::cout << std::fixed << std::setprecision(3);
    auto [seconds, result] = eval<T>(func);

    std::cout << "Day " << day << ": Part " << part << " = " << result << "\t\t (completed in " << seconds
              << "s)." << std::endl;
}

#endif //ADVENT_OF_CODE_2017_ADVENT_H
