#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

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
    static_assert(std::is_integral_v<std::iter_value_t<It>>);

    auto [max, min] = std::minmax_element(begin, end);
    return ((*min) - (*max));
}

// find the only two numbers in each row where one evenly divides the other and divide them
// assumes input is sorted ascending and contains a pair of numbers that evenly divide each other
template<typename It>
std::iter_value_t<It> even_divisors(It begin, It end) {
    static_assert(std::is_integral_v<std::iter_value_t<It>>);

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

/// Unit tests
#ifdef CATCH2
#include <catch2/catch_test_macros.hpp>
TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 3);

    SECTION("Checksum is 18") {
        // transform a collection c => max(c)-min(c)
        auto transform = [](auto s) { return max_minus_min(s.begin(), s.end()); };

        // adds all transformed input lines to give checksum
        int checksum = std::transform_reduce(input.lines.begin(), input.lines.end(),
                                             0,                 // initial value
                                             std::plus<int>{},  // add
                                             transform);        // calc checksums
        REQUIRE(checksum == 18);
    }

}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 3);

    SECTION("Sum of each row is 9") {
        // transform a collection c => max(c)-min(c)
        auto transform = [](const auto& s) { return even_divisors(s.begin(), s.end()); };

        // adds all transformed input lines to give checksum
        int checksum = std::transform_reduce(input.lines.begin(), input.lines.end(),
                                             0,                 // initial value
                                             std::plus<int>{},  // add
                                             transform);        // calc checksums
        REQUIRE(checksum == 9);
    }
}
#endif

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