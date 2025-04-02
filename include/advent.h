#ifndef ADVENT_H
#define ADVENT_H

#include <bits/stdc++.h>

using ReturnType = std::variant<int64_t, uint64_t, double, std::string>;

// external declarations
ReturnType partOne();
ReturnType partTwo();
const extern std::string day;

#endif //ADVENT_H