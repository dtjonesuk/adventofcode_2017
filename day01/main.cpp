#include "../advent.h"
#include <set>

/// Change this to the current day
static const std::string day = "01";
static const std::string inputFilename = "..\\day" + day + "\\input.txt";
static const std::string testFilename = "..\\day" + day + "\\test.txt";

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

/// Unit Test
void test() {
//    std::ifstream istream(testFilename);
    std::stringstream sstream{"91212129"};
    PuzzleInput input(sstream);

    assert(solve_captcha(input.lines[0]) == 9);

    std::string p2 = "12131415";
    assert(solve_captcha(p2, p2.length() / 2) == 4);
}


/// Part One Solution
int64_t partOne() {
    using namespace std::views;

    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    const std::string captcha = input.lines[0];
    return solve_captcha(captcha);
}


/// Part Two Solution
int64_t partTwo() {
    using namespace std::views;

    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    const std::string captcha = input.lines[0];

    return solve_captcha(captcha, captcha.length() / 2);
}


int main() {
    test();
    run<int64_t>(day, "One", partOne);
    run<int64_t>(day, "Two", partTwo);
}