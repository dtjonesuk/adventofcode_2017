#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";


struct Hex {
    int q, r, s;

    Hex(int q, int r, int s) : q(q), r(r), s(s) {}

    Hex() : Hex(0, 0, 0) {}

    int distance() {
        return std::max({q, r, s});
    }

    void move(const std::string &direction) {
        static const std::map<std::string, Hex> direction_vectors{
                {"n",  {0,  1,  -1}},
                {"s",  {0,  -1, 1}},
                {"ne", {1,  0,  -1}},
                {"sw", {-1, 0,  1}},
                {"nw", {-1, 1,  0}},
                {"se", {1,  -1, 0}},
        };
        const Hex &dir = direction_vectors.at(direction);
        *this = *this + dir;
    }

    Hex operator+(Hex other) const {
        return {q + other.q, r + other.r, s + other.s};
    }

    Hex operator-(Hex other) const {
        return {q - other.q, r - other.r, s - other.s};
    }

private:

};

std::ostream &operator<<(std::ostream &ostream, Hex &cube) {
    ostream << std::format("({}, {}, {})", cube.q, cube.r, cube.s);
    return ostream;
}


struct PuzzleInput {
    std::vector<std::string> directions;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line, ',')) {
            // do something
            directions.emplace_back(std::move(line));
        }
    }

};

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Move ne calculates correctly") {
        Hex hex;
        hex.move( "ne");
        hex.move( "ne");
        hex.move( "ne");
        REQUIRE(hex.distance() == 3);
    }
    SECTION("Move back to origin calculates correctly") {
        Hex hex;
        hex.move( "ne");
        hex.move( "ne");
        hex.move( "sw");
        hex.move( "sw");
        REQUIRE(hex.distance() == 0);
    }
    SECTION("Move two steps away calculates correctly") {
        Hex hex;
        hex.move( "ne");
        hex.move( "ne");
        hex.move( "s");
        hex.move( "s");
        REQUIRE(hex.distance() == 2);
    }
    SECTION("Move three steps away calculates correctly") {
        Hex hex;
        hex.move( "se");
        hex.move( "sw");
        hex.move( "se");
        hex.move( "sw");
        hex.move( "sw");
        REQUIRE(hex.distance() == 3);
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

    Hex hex;

    std::ranges::for_each(input.directions, [&hex](const auto &dir) { hex.move(dir); });

    return hex.distance();
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Hex hex;

    auto distances = input.directions | std::views::transform([&hex](const auto &dir) {
        hex.move(dir);
        return hex.distance();
    });


    auto max = std::ranges::fold_left(distances, 0, [](int a, int b) { return std::max(a, b); });

    return max;
}
