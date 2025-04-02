#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct Group {
    size_t score;
    size_t garbage;
    std::vector<Group> children;

    explicit Group(size_t score = 1) : score(score), garbage(0) {}
};

struct Parser {
    explicit Parser(const std::string &input) : input(input) {}

    explicit Parser(std::string &&input) : input(std::move(input)) {}

    std::string input;

    Group parse() {
        if (peek() == '{') {
            return group(1);
        }
        throw std::runtime_error("invalid input string");
    }

    Group group(size_t score) {
        Group grp(score);

        advance();

        // parse child group
        auto [garbage_len, children] = grouplist(score + 1);
        grp.garbage = garbage_len;
        grp.children = std::move(children);

        advance();

        return grp;
    }

    std::pair<size_t, std::vector<Group>> grouplist(size_t score) {
        std::vector<Group> children;
        size_t garbage_len{};
        while (!eof() && peek() != '}') {
            if (peek() == '{') {
                // add group to list
                children.emplace_back(group(score));
            } else if (peek() == '<') {
                // consume garbage
                garbage_len += garbage();
            }
            if (peek() == ',')
                advance();
        }
        return {garbage_len, children};
    }


    size_t garbage() {
        size_t len = 0;
        while (!eof()) {
            advance();
            if (peek() == '!') {
                // ignore next char
                advance();
            } else if (peek() == '>') {
                advance();
                return len;
            } else {
                ++len;
            }
        }
        throw std::runtime_error("Unexpected end of garbage");
    }

private:
    size_t current = 0;

    void advance() { if (current < input.size()) ++current; }
    void consume(char ch) { if (peek() != ch) throw std::runtime_error(std::format("Invalid token: {} expected {}", input[current], ch)); advance();}
    char peek() const { return input[current]; }
    bool eof() const { return current >= input.size(); }

};

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

int64_t count_groups(const Group &group) {
    return 1LL + std::transform_reduce(group.children.begin(), group.children.end(),
                                       0LL,
                                       std::plus{},
                                       count_groups);
}

size_t score_group(const Group &group) {
    return group.score + std::transform_reduce(group.children.begin(), group.children.end(),
                          0ULL,
                          std::plus{},
                          score_group);
}

size_t count_garbage(const Group &group) {
    return group.garbage + std::transform_reduce(group.children.begin(), group.children.end(),
                                                 0ULL,
                                                 std::plus{},
                                                 count_garbage);
}

size_t count_garbage(const std::string &input) {
    Parser parser(std::format("{{{}}}", input));
    return count_garbage(parser.parse());
}
/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 8);

    SECTION("Group count is correct") {
        const std::vector<size_t> correct_values = {1, 3, 3, 6, 1, 1, 5, 2};
        for (size_t current = 0; const std::string &line: input.lines) {
            Parser parser(line);
            Group g = parser.parse();
            REQUIRE(count_groups(g) == correct_values[current++]);
        }
    }

    SECTION("Group score is correct") {
        const std::vector<size_t> correct_values = {1, 6, 5, 16, 1, 1, 9, 3};
        for (size_t current = 0; const std::string &line: input.lines) {
            Parser parser(line);
            Group g = parser.parse();
            REQUIRE(score_group(g) == correct_values[current++]);
        }
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 8);

    SECTION("Random garbage count is correct") {
        REQUIRE(count_garbage("<>") == 0);
        REQUIRE(count_garbage("<random characters>") == 17);
        REQUIRE(count_garbage("<<<<>") == 3);
        REQUIRE(count_garbage("<{!>}>") == 2);
        REQUIRE(count_garbage("<!!>") == 0);
        REQUIRE(count_garbage("<!!!>>") == 0);
        REQUIRE(count_garbage("<{o\"i!a,<{i<a>") == 10);

    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Parser parser(input.lines[0]);
    Group g = parser.parse();

    return score_group(g);
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Parser parser(input.lines[0]);
    Group g = parser.parse();

    return count_garbage(g);
}
