#ifndef ADVENT_H
#define ADVENT_H

#include <string>
#include <iostream>
#include <fstream> 
#include <sstream>
#include <regex>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <utility>
#include <tuple>
#include <chrono>
#include <cstdint>
#include <cmath>
#include <cassert>

using ReturnType = std::variant<int64_t, uint64_t, double, std::string>;

// external declarations
ReturnType partOne();
ReturnType partTwo();
const extern std::string day;

#endif //ADVENT_H