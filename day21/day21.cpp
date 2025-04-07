#include <advent.h>


/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct Pattern {
    Pattern(size_t x, size_t y) : x_size(x), y_size(y) {}


    Pattern(const std::initializer_list<std::string> &input) {
        y_size = 0;

        for (const auto &line: input) {
            y_size++;
            for (const char c: line) {
                data.push_back(c == '#');
            }
        }
        
        x_size = data.size() / y_size;
    }
    
    explicit Pattern(std::string_view input) {
        y_size = 0;
        for (const auto &line: std::views::split(input, '/')) {
            ++y_size;
            for (const char c: line) {
                data.push_back(c == '#');
            }
        }
        x_size = data.size() / y_size;
    }

    explicit Pattern(const std::ranges::borrowed_range auto &input) {
        y_size = 0;

        for (const auto &line: input) {
            y_size++;
            for (const char c: line) {
                data.push_back(c == '#');
            }
        }

        x_size = data.size() / y_size;
    }

    size_t x_size;
    size_t y_size;
    std::vector<bool> data;
    
    
    bool at(size_t x, size_t y) {
        return data[y * x_size + x];
    }

    bool operator<(const Pattern &other) const {
        return std::tie(x_size, y_size, data) < std::tie(other.x_size, other.y_size, other.data);
    }
};

struct PuzzleInput {
    std::map<Pattern, Pattern> rules;
    const Pattern initial_state = Pattern({".#.",
                                           "..#",
                                           "###"});

    const std::regex input_re{R"((.*) => (.*))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            std::smatch smatch;
            if (std::regex_match(line, smatch, input_re)) {
                std::string key{smatch.str(1)};
                std::string value{smatch.str(2)};
//                std::cout << std::quoted(key) << " : " << std::quoted(value) << std::endl;
                rules.emplace(key, value);
            }
        }
    }

};

//int64_t rule(int64_t current) {
//    if (current % 2 == 0) {
//        // divisible by 2?
//        std::cout << current / 2 << " 2x2 -> " << current / 2 << " 3x3\n";
//        return current / 2 * 3;
//    } else {
//        // divisible by 3!
//        std::cout << current / 3 << " 3x3 -> " << current / 3 << " 4x4\n";
//        return current / 3 * 4;
//    }
//}

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Input parses correctly") {
        REQUIRE(input.rules.size() == 2);
        REQUIRE(input.initial_state.x_size == 3);
        REQUIRE(input.initial_state.y_size == 3);
        REQUIRE(std::ranges::equal(input.initial_state.data,
                                   std::initializer_list<bool>{false, true, false, false, false, true, true, true,
                                                               true}));
        REQUIRE(std::ranges::equal(Pattern(".#./..#/###").data, input.initial_state.data));
    }
    
    SECTION("") {

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
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    return 0;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    return 0;
}
