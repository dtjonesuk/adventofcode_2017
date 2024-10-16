#include "../advent.h"

/// Change this to the current day
static const std::string day = "##";
static const std::string inputFilename = "..\\day" + day + "\\input.txt";
static const std::string testFilename = "..\\day" + day + "\\test.txt";

struct PuzzleInput {
    std::vector <std::string> lines;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            lines.emplace_back(std::move(line));
        }
    }

};

/// Unit Test
void test() {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);
    assert(true);
}

/// Part One Solution
int64_t partOne() {
    using namespace std::views;

    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    return 0;
}


/// Part Two Solution
int64_t partTwo() {
    using namespace std::views;

    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    return 0;
}


int main() {
    test();
    run<int64_t>(day, "One", partOne);
    run<int64_t>(day, "Two", partTwo);
}