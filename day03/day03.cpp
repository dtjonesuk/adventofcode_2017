#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

const int input = 325489;

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

std::pair<int, int> get_coord(int n) {
    if (n < 2)
        return {0, 0};

    // what layer is n in?
    int layer = std::ceil((std::sqrt(n) - 1) / 2);

    // what is size of layer?
    constexpr auto layer_size = [](int layer) { return (layer * 2 + 1) * (layer * 2 + 1); };
    int layer_max = layer_size(layer);

    // where is n in layer?
    int layer_perimeter = layer_max - layer_size(layer - 1);
    int pos_in_layer = n - layer_size(layer - 1) - 1;//layer_max - n;

    int x = layer;
    int y = -(layer - 1);

    int side_size = layer_perimeter / 4;
    switch (pos_in_layer / side_size) {
        case 0:
            y += pos_in_layer;
            break;
        case 1:
            y += side_size + 1;
            x -= pos_in_layer - side_size - 1;
            break;
        case 2:
            x -= side_size;
            y -= side_size + 1;
            y += (pos_in_layer - (2 * side_size));
            break;
        case 3:
            ++y;
            x -= (4 * side_size) - (pos_in_layer) + 1;
    }

    // what is coord of pos in layer?
//        while (pos_in_layer > (side_size))
//            pos_in_layer -= (layer_perimeter / 4);

    int mid = ((layer_perimeter / 4) + 1) / 2;

    // what is distance to centre of coord?
//    int y = pos_in_layer - mid;
//    int x = layer;

    return {x, y};
}

auto manhattan_distance(int n) {
//    // what layer is n in?
//    int layer = std::ceil((std::sqrt(n) - 1) / 2);
//
//    // what is size of layer?
//    constexpr auto layer_size = [](int layer) { return (layer * 2 + 1) * (layer * 2 + 1); };
//    int layer_max = layer_size(layer);
//
//    // where is n in layer?
//    int pos_in_layer = layer_max - n;
//    int layer_perimeter = layer_max - layer_size(layer - 1);
//
//    // what is coord of pos in layer?
//    while (pos_in_layer > (layer_perimeter / 4))
//        pos_in_layer -= (layer_perimeter / 4);
//    int mid = ((layer_perimeter / 4) + 1) / 2;
//
//    // what is distance to centre of coord?
//    int y = std::abs(pos_in_layer - mid);
//    int x = layer;

    auto [x, y] = get_coord(n);
    return std::abs(x) + std::abs(y);
}

struct Cache {


    using Vec2 = std::pair<int, int>;

    Vec2 operator[](int n) {
        // if cached, return value
        if (cache.contains(n)) {
            return cache[n];
        }
        return {};
    }

    void fill() {
        int x = 0;
        int y = 0;
        int layer = 1;
        int layer_size = 1;
        int n = 1;

        for (int i = 0; i < 1'000'000; ++i) {
            cache[i] = {x, y};

            if (n == layer_size) {
                n = 1;
                ++layer;
                layer_size = (layer * 2 + 1) * (layer * 2 + 1);

                ++x;
            }

        }
    }

private:
    std::map<int, Vec2> cache;
};


auto sum_adjacent(int n) {


    // work out the # of the 8 adjacent squares

    // work out which adjacent squares already have values

    // return sum of adjacent squares with values
    return 0;
}
/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

//    REQUIRE(input.lines.size() == 1);

    SECTION("Manhattan distances compute correctly") {
        REQUIRE(manhattan_distance(1) == 0);
        REQUIRE(manhattan_distance(12) == 3);
        REQUIRE(manhattan_distance(23) == 2);
        REQUIRE(manhattan_distance(1024) == 31);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    Cache cache;
    cache.fill();
//    REQUIRE(input.lines.size() == 1);

    SECTION("Sum of the adjacent squares computes correctly") {
//        REQUIRE(sum_adjacent(1) == 1);
//        REQUIRE(sum_adjacent(2) == 1);
//        REQUIRE(sum_adjacent(3) == 2);
//        REQUIRE(sum_adjacent(4) == 4);
//        REQUIRE(sum_adjacent(5) == 4);
    }
}

#endif


/// Part One Solution
int64_t partOne() {
    return manhattan_distance(input);
}


/// Part Two Solution
int64_t partTwo() {


    return 0;
}
