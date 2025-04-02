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

int solve_captcha(const std::string &captcha, const ptrdiff_t step = 1) {
    assert(captcha.length() > 0);               // must have non-empty string

    int total = 0;
    for (auto it = std::begin(captcha); it != std::end(captcha); ++it) {
        auto nxt = std::next(it, step);
        // wrap around end to beginning.
        if (nxt >= std::end(captcha)) {
            nxt = std::next(std::begin(captcha), step) - std::distance(it, std::end(captcha));
        }

        if (*it == *nxt) {
            // convert digit to int
            int n = *it - '0';
            total += n;
        }
    }
    return total;
}

/// Unit tests
#ifdef CATCH2
#include <catch2/catch_test_macros.hpp>
TEST_CASE("Part One Tests") {
    std::stringstream sstream{"91212129"};
    PuzzleInput input(sstream);

    REQUIRE(input.lines.size() == 1);


    SECTION("") {
        REQUIRE(solve_captcha(input.lines[0]) == 9);


        std::string p2 = "12131415";
        REQUIRE(solve_captcha(p2, p2.length() / 2) == 4);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

}
#endif


/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    const std::string captcha = input.lines[0];
    return solve_captcha(captcha);
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    const std::string captcha = input.lines[0];

    return solve_captcha(captcha, captcha.length() / 2);
}

