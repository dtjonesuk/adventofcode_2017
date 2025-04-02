#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct Generator {
    int64_t initial_value;
    int64_t factor;
    int64_t multiple;
    std::string name;
};

struct PuzzleInput {
    std::vector<Generator> generators;

    const std::regex input_re{R"(Generator (\w) starts with (\d+))"};
//    static const int64_t A = 16807;
//    static const int64_t B = 48271;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            std::smatch smatch;
            if (std::regex_match(line, smatch, input_re)) {
                std::string name = smatch.str(1);
                int64_t factor = name == "A" ? 16807 : 48271;
                int64_t multiple = name == "A" ? 4 : 8;
                int64_t initial_value = stoi(smatch.str(2));
                generators.emplace_back(initial_value, factor, multiple, name);
            }
        }
    }

};

int64_t generate(int64_t value, int64_t factor) {
    int64_t product = (value * factor) % 2147483647;
    return product;
}

int64_t generate(int64_t value, int64_t factor, int64_t multiple) {
    do {
        value = generate(value, factor);
    } while (value % multiple != 0);
    return value;
}

// Part One generator function
std::vector<int64_t> generator(const Generator &gen, int n) {
    std::vector<int64_t> results;
    int64_t value = gen.initial_value;
    for (int i = 0; i < n; ++i) {
        value = generate(value, gen.factor);
        results.push_back(value);
    }
    return results;
}

// Part Two generator function
std::vector<int64_t> generator2(const Generator &gen, int n) {
    std::vector<int64_t> results;
    int64_t value = gen.initial_value;
    for (int i = 0; i < n; ++i) {
        do {
            value = generate(value, gen.factor);
        } while (value % gen.multiple != 0);
        results.push_back(value);
    }
    return results;
}


bool judge(int64_t a, int64_t b) {
    return (a & 0xffff) == (b & 0xffff);
}

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Input parses correctly") {
        REQUIRE(input.generators.size() == 2);
        REQUIRE(input.generators[0].initial_value == 65);
        REQUIRE(input.generators[1].initial_value == 8921);
    }

    SECTION("Generator A computes correct results") {
        std::vector<int64_t> results = generator(input.generators[0], 5);
        std::initializer_list<int64_t> answers{1092455, 1181022009, 245556042, 1744312007, 1352636452};

        REQUIRE(std::ranges::equal(results, answers));
    }

    SECTION("Generator B computes correct results") {
        std::vector<int64_t> results = generator(input.generators[1], 5);
        std::initializer_list<int64_t> answers{430625591, 1233683848, 1431495498, 137874439, 285222916};

        REQUIRE(std::ranges::equal(results, answers));
    }

    SECTION("Judge function computes correct results") {
        std::vector<int64_t> resultsA = generator(input.generators[0], 5);
        std::vector<int64_t> resultsB = generator(input.generators[1], 5);
        std::vector<bool> results;
        std::ranges::transform(resultsA, resultsB, back_inserter(results), judge);
        std::initializer_list<bool> answers{false, false, true, false, false};

        REQUIRE(std::ranges::equal(results, answers));
    }

    SECTION("Judge function computes correct results") {
        const Generator &genA = input.generators[0];
        const Generator &genB = input.generators[1];

        int64_t value_a = genA.initial_value;
        int64_t value_b = genB.initial_value;

        int64_t count = 0;

        for (int i = 0; i < 40'000'000; ++i) {
            value_a = generate(value_a, genA.factor);
            value_b = generate(value_b, genB.factor);
            if (judge(value_a, value_b))
                ++count;
        }
        REQUIRE(count == 588);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Generator A computes correct results") {
        std::vector<int64_t> results = generator2(input.generators[0], 5);
        std::initializer_list<int64_t> answers{1352636452, 1992081072, 530830436, 1980017072, 740335192};

        REQUIRE(std::ranges::equal(results, answers));
    }

    SECTION("Generator B computes correct results") {
        std::vector<int64_t> results = generator2(input.generators[1], 5);
        std::initializer_list<int64_t> answers{1233683848, 862516352, 1159784568, 1616057672, 412269392};

        REQUIRE(std::ranges::equal(results, answers));
    }

    SECTION("Judge function computes correct results") {
        const Generator &genA = input.generators[0];
        const Generator &genB = input.generators[1];

        int64_t value_a = genA.initial_value;
        int64_t value_b = genB.initial_value;

        int64_t count = 0;

        for (int i = 0; i < 5'000'000; ++i) {
            value_a = generate(value_a, genA.factor, genA.multiple);
            value_b = generate(value_b, genB.factor, genB.multiple);

            if (judge(value_a, value_b))
                ++count;
        }
        REQUIRE(count == 309);
    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    const Generator &genA = input.generators[0];
    const Generator &genB = input.generators[1];

    int64_t value_a = genA.initial_value;
    int64_t value_b = genB.initial_value;

    int64_t count = 0;

    for (int i = 0; i < 40'000'000; ++i) {
        value_a = generate(value_a, genA.factor);
        value_b = generate(value_b, genB.factor);
        if (judge(value_a, value_b))
            ++count;
    }

    return count;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    const Generator &genA = input.generators[0];
    const Generator &genB = input.generators[1];

    int64_t value_a = genA.initial_value;
    int64_t value_b = genB.initial_value;

    int64_t count = 0;

    for (int i = 0; i < 5'000'000; ++i) {
        value_a = generate(value_a, genA.factor, genA.multiple);
        value_b = generate(value_b, genB.factor, genB.multiple);

        if (judge(value_a, value_b))
            ++count;
    }

    return count;
}
