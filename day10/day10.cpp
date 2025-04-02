#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct String {
    int size;
    std::vector<int> numbers;

    explicit String(int size) : size(size), numbers(size) {
        std::iota(begin(numbers), end(numbers), 0);
    }


    void reverse(size_t start, size_t count) {
        if (count <= 1)
            return;

        for (size_t from = start, to = (start + count - 1) % size; count > 0;) {
            std::swap(numbers[to], numbers[from]);

            if (to == 0)
                to = size - 1;
            else
                to--;

            from++;
            if (from == size)
                from = 0;

            if (count < 2)
                count = 0;
            else
                count -= 2;
        }
    }

    bool equals(const std::initializer_list<int> &list) {
        return std::equal(begin(numbers), end(numbers), begin(list), end(list));
    }

    int hashcode(const std::vector<int> &lengths) {
        int current = 0;
        int skip = 0;
        for (int len: lengths) {
            reverse(current, len);
            current = (current + len + skip) % size;
            skip++;
        }
        return numbers[0] * numbers[1];
    }

    std::string hashcode2(const std::string &input) {
        std::vector<int> lengths;

        for (unsigned char c: input) {
            lengths.push_back(c);
        }
        for (int i: {17, 31, 73, 47, 23})
            lengths.push_back(i);

        // 64 rounds of knot twisting
        int current = 0;
        int skip = 0;
        for (int i = 0; i < 64; ++i) {
            for (int len: lengths) {
                reverse(current, len);
                current = (current + len + skip) % size;
                skip++;
            }
        }

        // xor the 16-digit blocks
        auto knothash = std::ranges::views::iota(0, 16) | std::views::transform([this](unsigned char i){
            auto block = std::views::counted(begin(numbers) + (i*16), 16);
            unsigned char sum = std::ranges::fold_left_first(block, std::bit_xor{}).value();
            return sum;
        });

        std::stringstream ss;

        for (int n : knothash) {
            ss << std::format("{:02x}", n);
        }

        return ss.str();
    }
};

struct PuzzleInput {
    std::vector<int> lengths;
    std::vector<std::string> lines;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line, ',')) {
            // do something
            lengths.emplace_back(std::stoi(line));
        }
    }

    explicit PuzzleInput(std::istream &istream, bool partTwo) {
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

    REQUIRE(input.lengths.size() == 4);

    SECTION("Numbers initialize correctly") {
        String string(256);
        REQUIRE(string.numbers.size() == 256);
        for (int i = 0; i < 256; i++) {
            REQUIRE(string.numbers[i] == i);
        }
    }

    SECTION("Numbers initialize correctly") {
        String string(5);
        REQUIRE(string.numbers.size() == 5);
        for (int i = 0; i < 5; i++) {
            REQUIRE(string.numbers[i] == i);
        }
    }

    SECTION("Numbers reverse correctly") {
        String string(5);

        string.reverse(0, 3);
        REQUIRE(string.equals({2, 1, 0, 3, 4}));

        string.reverse(3, 4);
        REQUIRE(string.equals({4, 3, 0, 1, 2}));

        string.reverse(4, 1);
        REQUIRE(string.equals({4, 3, 0, 1, 2}));

        string.reverse(3, 5);
        REQUIRE(string.equals({4, 2, 1, 0, 3}));
    }

    SECTION("Hash computes correctly") {
        String string(5);

        REQUIRE(string.hashcode(input.lengths) == 12);
        REQUIRE(string.equals({3, 4, 2, 1, 0}));
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream, true);

    REQUIRE(input.lines.size() == 1);

    SECTION("Empty string hashcode computes correctly") {
        String string(256);
        REQUIRE(string.hashcode2("") == "a2582a3a0e66e6e86e3812dcb672a272");
    }
    SECTION("Aoc 2017 hashcode computes correctly") {
        String string(256);
        REQUIRE(string.hashcode2("AoC 2017") == "33efeb34ea91902bb2f59c9920caa6cd");
    }
    SECTION("1,2,3 hashcode computes correctly") {
        String string(256);
        REQUIRE(string.hashcode2("1,2,3") == "3efbe78a8d82f29979031a4aa0b16a9d");
    }
    SECTION("1,2,4 hashcode computes correctly") {
        String string(256);
        REQUIRE(string.hashcode2("1,2,4") == "63960835bcdc130f0b66d7ff4f6a5a8e");
    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    String string(256);

    return string.hashcode(input.lengths);
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream, true);

    String string(256);
    std::cout << string.hashcode2(input.lines[0]) << std::endl;
    return 0;
}
