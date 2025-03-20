#include <advent.h>

/// Change this to the current day
extern const std::string day = "##";
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct PuzzleInput {
    std::vector<int> jumps;
    size_t current_instruction = 0;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            jumps.emplace_back(stoi(line));
        }
    }

    /// Executes the next instruction
    /// returns true, if execute continues, false if outside bounds
    bool execute() {
        int jump = jumps[current_instruction];
        jumps[current_instruction]++;
        current_instruction += jump;

        return current_instruction < jumps.size();
    }

    /// Executes the next instruction
    /// returns true, if execute continues, false if outside bounds
    bool execute2() {
        int jump = jumps[current_instruction];
        if (jump >= 3)
            jumps[current_instruction]--;
        else
            jumps[current_instruction]++;

        current_instruction += jump;

        return current_instruction < jumps.size();
    }

};

/// Unit tests
#ifdef CATCH2
#include <catch2/catch_test_macros.hpp>
TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.jumps.size() == 5);

    SECTION("Jump instructions execute correctly") {
        REQUIRE(input.execute());
        REQUIRE(input.jumps[0] == 1);
        REQUIRE(input.current_instruction == 0);

        REQUIRE(input.execute());
        REQUIRE(input.jumps[0] == 2);
        REQUIRE(input.current_instruction == 1);

        REQUIRE(input.execute());
        REQUIRE(input.jumps[1] == 4);
        REQUIRE(input.current_instruction == 4);

        REQUIRE(input.execute());
        REQUIRE(input.jumps[4] == -2);
        REQUIRE(input.current_instruction == 1);

        REQUIRE_FALSE(input.execute());
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.jumps.size() == 5);

    SECTION("Jump instructions execute correctly") {
        int64_t count = 0;
        while (input.execute2()) {
            ++count;
        }
        ++count;

        REQUIRE(count == 10);
        REQUIRE(input.jumps[0] == 2);
        REQUIRE(input.jumps[1] == 3);
        REQUIRE(input.jumps[2] == 2);
        REQUIRE(input.jumps[3] == 3);
        REQUIRE(input.jumps[4] == -1);

    }
}
#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    int64_t count = 0;

    while (input.execute()) {
        ++count;
    }

    // we have to include the last execute step that wasn't counted
    return count + 1;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    int64_t count = 0;

    while (input.execute2()) {
        ++count;
    }

    // we have to include the last execute step that wasn't counted
    return count + 1;
}
