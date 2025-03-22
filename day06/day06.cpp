#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct PuzzleInput {
    std::vector<int> banks;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;
        while (std::getline(istream, line, '\t')) {
            // do something
            banks.push_back(stoi(line));
        }
    }

    void redistribute() {
        // find the bank with most blocks
        // first bank wins ties

        auto max = std::max_element(banks.begin(), banks.end());

        // calculate the amount to distribute to each bank
        int amount = *max / int(banks.size());

        // calculate # of banks that get an extra block (remainder)
        int extra = *max % int(banks.size());

        // starting with next bank, spread the blocks out over the memory banks
        auto it = next(max);
        if (it == banks.end())
            it = banks.begin();

        do {
            *it += amount;
            if (extra) {
                *it = *it + 1;
                --extra;
            }

            it++;
            if (it == banks.end())
                it = banks.begin();
        } while (it != max);
        *max = amount;
    }

};

int64_t count_redistributions(PuzzleInput& input) {
    std::set<std::vector<int>> cache;
    int64_t count = 0;

    while(true) {
        if (cache.contains(input.banks))
            return count;
        cache.insert(input.banks);
        input.redistribute();
        ++count;
    }
}

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.banks.size() == 4);

    SECTION("Redistribute blocks computes correctly") {
        REQUIRE(input.banks == std::vector<int>{0, 2, 7, 0});
        input.redistribute();
        REQUIRE(input.banks == std::vector<int>{2, 4, 1, 2});
        input.redistribute();
        REQUIRE(input.banks == std::vector<int>{3, 1, 2, 3});
        input.redistribute();
        REQUIRE(input.banks == std::vector<int>{0, 2, 3, 4});
        input.redistribute();
        REQUIRE(input.banks == std::vector<int>{1, 3, 4, 1});
        input.redistribute();
        REQUIRE(input.banks == std::vector<int>{2, 4, 1, 2});
    }

    SECTION("Count of redistributions computes correctly") {
        REQUIRE(count_redistributions(input) == 5);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.banks.size() == 4);

    SECTION("Count of loop size computes correctly") {
        count_redistributions(input);
        REQUIRE(count_redistributions(input) == 4);
    }
}

#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = count_redistributions(input);
    return n;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = count_redistributions(input);
    n = count_redistributions(input);
    return n;
}
