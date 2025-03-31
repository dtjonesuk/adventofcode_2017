#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";


struct CircularBuffer {
    CircularBuffer() {
        current = std::begin(data);
    }

    std::list<int> data;
    std::list<int>::iterator current;

    std::list<int>::iterator next() {
        ++current;
        // if current is at end, wrap around to begin
        if (current == std::end(data))
            current = std::begin(data);

        return current;
    }

    std::list<int>::iterator forward(size_t n) {
        for (int i = 0; i < n; ++i)
            next();

        return current;
    }

    std::list<int>::iterator insert(int value) {
        // insert value after current position
        current = data.insert(++current, value);

        // if current position is at end, wrap around to begin
        if (current == std::end(data))
            current = std::begin(data);

        return current;
    }
};

struct PuzzleInput {
    std::vector<std::string> lines;
    const int step = 380;
    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            lines.emplace_back(std::move(line));
        }
    }

};

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);



    SECTION("CircularBuffer inserts numbers correctly") {
        CircularBuffer buffer;

        std::initializer_list<int> answers[] = {
                {0},
                {0, 1},
                {0, 2, 1},
                {0, 2, 3, 1},
                {0, 2, 4, 3, 1},
                {0, 5, 2, 4, 3, 1},
                {0, 5, 2, 4, 3, 6, 1},
                {0, 5, 7, 2, 4, 3, 6, 1},
                {0, 5, 7, 2, 4, 3, 8, 6, 1},
                {0, 9, 5, 7, 2, 4, 3, 8, 6, 1},
        };

        // initial value is 0
        buffer.insert(0);

        REQUIRE(std::ranges::equal(buffer.data, answers[0]));

        for (int i = 1; i < 10; ++i) {
            // move forward through buffer and insert next number
            buffer.forward(3);
            buffer.insert(i);

            CHECK(std::ranges::equal(buffer.data, answers[i]));
        }
    }

    SECTION("CircularBuffer computes correct value after 2017 iterations") {
        CircularBuffer buffer;

        for (int i = 0; i <= 2017; ++i) {
            buffer.forward(3);
            buffer.insert(i);
        }

        REQUIRE(*buffer.current == 2017);
        REQUIRE(*buffer.next() == 638);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);


    SECTION("") {

    }
}

#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    CircularBuffer buffer;

    for (int i = 0; i <= 2017; ++i) {
        buffer.forward(input.step);
        buffer.insert(i);
    }

    return *buffer.next();
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    CircularBuffer buffer;

    for (int i = 0; i <= 50'000'000; ++i) {
        buffer.forward(input.step);
        buffer.insert(i);
    }

    auto it = std::ranges::find(buffer.data, 0);

    return *(++it);
}
