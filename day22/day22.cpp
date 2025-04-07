#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct Coord {
    int x;
    int y;

    Coord(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator<(const Coord &other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

class Direction {
public:
    enum Compass {
        North,
        East,
        South,
        West
    };
    enum LR {
        Left,
        Right
    };

    Direction(Compass initial) : direction(initial) {}

    Compass direction;

    void turn(LR way) {
        static std::array ways = {North, East, South, West};
        int n = static_cast<int>(direction);
        if (way == LR::Left) {
            n -= 1;
            if (n < 0)
                n = 3;
        } else {
            n += 1;
            if (n > 3)
                n = 0;
        }
        direction = static_cast<Compass>(n);
    }

    [[nodiscard]] Coord forward(const Coord &coord) const {
        switch (direction) {
            case North:
                return {coord.x, coord.y - 1};
            case East:
                return {coord.x + 1, coord.y};
            case South:
                return {coord.x, coord.y + 1};
            case West:
                return {coord.x - 1, coord.y};
            default:
                throw std::runtime_error("Invalid forward direction!");
        }
    }

    Compass operator()() const {
        return direction;
    }
};


struct Map {
    enum Status {
        Clean,
        Weakened,
        Infected,
        Flagged
    };

    std::map<Coord, Status> nodes;
    Coord current{0, 0};
    Direction direction{Direction::Compass::North};

    explicit Map(auto &input) {
        for (int y = 0; auto &line: input) {
            for (int x = 0; char c: line) {
                nodes.emplace(Coord{x, y}, c == '#' ? Infected : Clean);
                ++x;
            }
            ++y;
        }

        // start in the middle of the map
        current = {int(input.size() / 2), int(input[0].size() / 2)};
    }

    // Part One
    int64_t burst_1(int64_t times) {
        int64_t n = 0;
        for (int64_t i = 0; i < times; ++i) {
            if (burst_1())
                n++;
        }
        return n;
    }

    bool burst_1() {
        if (nodes[current] == Infected) {
            // currently infected
            direction.turn(Direction::LR::Right);
        } else {
            // currently not infected
            direction.turn(Direction::LR::Left);
        }
        bool infected = nodes[current] == Clean;
        nodes[current] = infected ? Infected : Clean;
        current = direction.forward(current);
        return infected;
    }

    // Part Two
    int64_t burst_2(int64_t times) {
        int64_t n = 0;
        for (int64_t i = 0; i < times; ++i) {
            if (burst_2())
                n++;
        }
        return n;
    }
    
    bool burst_2() {
        bool infected = false;
        switch (nodes[current]) {
            case Clean:
                direction.turn(Direction::LR::Left);
                nodes[current] = Weakened;
                break;
            case Weakened:
                infected = true;
                nodes[current] = Infected;
                break;
            case Infected:
                direction.turn(Direction::LR::Right);
                nodes[current] = Flagged;
                break;
            case Flagged:
                // reverse direction by turning R twice
                direction.turn(Direction::LR::Right);
                direction.turn(Direction::LR::Right);
                nodes[current] = Clean;
                break;
        }

        current = direction.forward(current);
        return infected;
    }
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

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.lines.size() == 3);
    Map map(input.lines);
    
    SECTION("Input parses correctly") {
    REQUIRE(std::ranges::count(map.nodes, Map::Status::Infected, &std::pair<const Coord, Map::Status>::second) == 2);
        REQUIRE(std::ranges::count(map.nodes, Map::Status::Clean, &std::pair<const Coord, Map::Status>::second) == 7);
    }

    SECTION("Burst 7 processes correctly") {
        REQUIRE(map.burst_1(7) == 5);
    }
    
    SECTION("Burst 70 processes correctly") {
        REQUIRE(map.burst_1(70) == 41);
    }
    
    SECTION("Burst 10,000 processes correctly") {
        REQUIRE(map.burst_1(10'000) == 5587);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    Map map(input.lines);
    
    SECTION("Burst 100 processes correctly") {
        REQUIRE(map.burst_2(100) == 26);
    }    
    
    SECTION("Burst 10,000,000 processes correctly") {
        REQUIRE(map.burst_2(10'000'000) == 2511944);
    }

}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);
    Map map(input.lines);

    return map.burst_1(10000);
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Map map(input.lines);

    return map.burst_2(10'000'000);
}
