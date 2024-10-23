#include "../advent.h"
#include <set>

/// Change this to the current day
static const std::string day = "02";
static const std::string inputFilename = "..\\day" + day + "\\input.txt";
static const std::string testFilename = "..\\day" + day + "\\test.txt";

struct PuzzleInput {
    std::vector<std::set<int>> lines;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;
        while (std::getline(istream, line)) {
            std::stringstream ss{line};
            std::set<int> values;
            std::string value;
            while (std::getline(ss, value, '\t')) {
                values.insert(stoi(value));
            }
            lines.push_back(values);
        }
    }

};

// calculate difference between max and min elements in a range
template<typename It>
std::iter_value_t<It> max_minus_min(It begin, It end) {
    static_assert(std::is_integral<std::iter_value_t<It>>());

    auto [max, min] = std::minmax_element(begin, end);
    return ((*min) - (*max));
}

// find the only two numbers in each row where one evenly divides the other and divide them
// assumes input is sorted ascending and contains a pair of numbers that evenly divide each other
template<typename It>
std::iter_value_t<It> even_divisors(It begin, It end) {
    static_assert(std::is_integral<std::iter_value_t<It>>());

    for (auto i = begin; i != end; ++i) {
        for (auto j = std::next(i); j != end; ++j) {
            if (*j % *i == 0)
                return *j / *i;
        }
    }
    // should never reach
    assert(false);
    return 0;
}

/// Unit Test
void test() {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    // transform a collection c => max(c)-min(c)
    auto transform = [](auto s) { return even_divisors(s.begin(), s.end()); };

    // adds all transformed input lines to give checksum
    int checksum = std::transform_reduce(input.lines.begin(), input.lines.end(),
                                         0,                 // initial value
                                         std::plus<int>{},  // add
                                         transform);        // calc checksums
    assert(checksum == 9);
}

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    // transform a collection c => max(c)-min(c)
    auto transform = [](auto s) { return max_minus_min(s.begin(), s.end()); };

    // adds all transformed input lines to give checksum
    int checksum = std::transform_reduce(input.lines.begin(), input.lines.end(),
                                         0,                 // initial value
                                         std::plus<int>{},  // add
                                         transform);        // calc checksums
    return checksum;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto transform = [](auto s) { return even_divisors(s.begin(), s.end()); };

    int checksum = std::transform_reduce(input.lines.begin(), input.lines.end(),
                                         0,
                                         std::plus<int>{},
                                         transform);

    return checksum;
}


int main() {
    test();
    run<int64_t>(day, "One", partOne);
    run<int64_t>(day, "Two", partTwo);
}