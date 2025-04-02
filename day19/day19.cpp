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

struct Coord {
    Coord(int x, int y) : x(x), y(y) {}

    int x = 0;
    int y = 0;

    bool operator<(const Coord &other) {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }

    Coord &operator+=(const Coord &other) {
        x = x + other.x;
        y = y + other.y;
        return *this;
    }

};

Coord &operator+(Coord &a, const Coord &b) {
    a += b;
    return a;
}

std::pair<std::string, int> find_word(const PuzzleInput &input) {
    std::string word;

    int max_y = (int)input.lines.size() - 1;
    int max_x = (int)input.lines[0].size() - 1;

    // find starting position
    int start = (int)distance(input.lines[0].begin(), std::ranges::find(input.lines[0], '|'));

    // set initial position, first row
    Coord position(start, 0);

    // set initial direction, straight down x=0, y=1
    Coord direction(0, 1);
    int count = 0;

    while (!(position.x < 0 || position.x > max_x || position.y < 0 || position.y > max_y)) {
        const char &current = input.lines[position.y][position.x];
        ++count;

        if (isalpha(current)) {
            // if letter then record letter
            word += current;
        } else if (current == ' ') {
            // if we reached the end, return
            --count;
            break;
        } else if (current == '+') {
            // if + then change direction (determine which way is valid)
            if (direction.x != 0) {
                if ((position.y < max_y) && (input.lines[position.y + 1][position.x] == '|' ||
                                             isalpha(input.lines[position.y + 1][position.x])))
                    direction = {0, 1};
                else if ((position.y > 0) && (input.lines[position.y - 1][position.x] == '|' ||
                                              isalpha(input.lines[position.y - 1][position.x])))
                    direction = {0, -1};
                else
                    throw std::runtime_error(
                            std::format("Unable to determine direction ({},{})", position.x, position.y));

            } else {
                if ((position.x < max_x) && (input.lines[position.y][position.x + 1] == '-' ||
                                             isalpha(input.lines[position.y][position.x + 1])))
                    direction = {1, 0};
                else if ((position.x > 0) && (input.lines[position.y][position.x - 1] == '-' ||
                                              isalpha(input.lines[position.y][position.x - 1])))
                    direction = {-1, 0};
                else
                    throw std::runtime_error(
                            std::format("Unable to determine direction ({},{})", position.x, position.y));
            }
        }

        // proceed in direction
        position = position + direction;
    }
    return {word, count};
}


/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    

    SECTION("Input formatting is correct") {
        REQUIRE(input.lines.size() == 6);
        REQUIRE (std::ranges::all_of(input.lines, [](const auto &s) { return s.size() == 15;}));
    }

    SECTION("Path is navigated correctly") {


        // find starting position
        int start = distance(input.lines[0].begin(), std::ranges::find(input.lines[0], '|'));
        REQUIRE(start == 5);

        auto [word, count] = find_word(input);
        REQUIRE(word == "ABCDEF");
    }  
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Path is navigated correctly") {


        // find starting position
        int start = distance(input.lines[0].begin(), std::ranges::find(input.lines[0], '|'));
        REQUIRE(start == 5);

        auto [word, count] = find_word(input);
        REQUIRE(count == 38);
    }  
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto [word, count] = find_word(input);
    std::cout << word << std::endl;

    return 0;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto [word, count] = find_word(input);
    return count;
}
