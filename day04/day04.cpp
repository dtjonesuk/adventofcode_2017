#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct PuzzleInput {
    std::vector<std::string> lines;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            lines.emplace_back(std::move(line));
        }
    }

};

bool is_passphrase_valid(const std::string& passphrase) {
    std::set<std::string> words;

    std::stringstream ss{passphrase};
    std::string w;
    while (ss >> w) {
        if (words.contains(w))
            return false;
        words.insert(w);
    }
    return true;
}

bool is_passphrase_valid_anagram(const std::string& passphrase) {
    std::set<std::string> words;

    std::stringstream ss{passphrase};
    std::string w;
    while (ss >> w) {
        std::sort(w.begin(), w.end());
        if (words.contains(w))
            return false;
        words.insert(w);
    }
    return true;
}


/// Unit tests
#ifdef CATCH2
#include <catch2/catch_test_macros.hpp>
TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 0);

    SECTION("Duplicate words detected") {
        REQUIRE(is_passphrase_valid("aa bb cc dd ee"));
        REQUIRE_FALSE(is_passphrase_valid("aa bb cc dd aa"));
        REQUIRE(is_passphrase_valid("aa bb cc dd aaa"));
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 0);

    SECTION("") {

    }
}
#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = std::count_if(input.lines.begin(), input.lines.end(), is_passphrase_valid);
    return n;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = std::count_if(input.lines.begin(), input.lines.end(), is_passphrase_valid_anagram);
    return n;
}
