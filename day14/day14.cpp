#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

const std::string puzzleInput = "wenycdww";

// From day10
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
        auto knothash = std::ranges::views::iota(0, 16) | std::views::transform([this](unsigned char i) {
            auto block = std::views::counted(begin(numbers) + (i * 16), 16);
            unsigned char sum = std::ranges::fold_left_first(block, std::bit_xor{}).value();
            return sum;
        });

        std::stringstream ss;

        for (int n: knothash) {
            ss << std::format("{:02x}", n);
        }

        return ss.str();
    }
};

std::string hex2bin(const std::string &hex) {
    std::stringstream ss;
    for (char c: hex) {
        switch (c) {
            case '0':
                ss << "0000";
                break;
            case '1':
                ss << "0001";
                break;
            case '2':
                ss << "0010";
                break;
            case '3':
                ss << "0011";
                break;
            case '4':
                ss << "0100";
                break;
            case '5':
                ss << "0101";
                break;
            case '6':
                ss << "0110";
                break;
            case '7':
                ss << "0111";
                break;
            case '8':
                ss << "1000";
                break;
            case '9':
                ss << "1001";
                break;
            case 'a':
            case 'A':
                ss << "1010";
                break;
            case 'b':
            case 'B':
                ss << "1011";
                break;
            case 'c':
            case 'C':
                ss << "1100";
                break;
            case 'd':
            case 'D':
                ss << "1101";
                break;
            case 'e':
            case 'E':
                ss << "1110";
                break;
            case 'f':
            case 'F':
                ss << "1111";
                break;
            default:
                ss << "xxxx";
                break;
        }
    }
    return ss.str();
}

int64_t popcnt(const std::string &bits) {
    return std::ranges::count_if(bits, [](char c) { return c == '1'; });
}

auto hash_square(const std::string &input) {
    using namespace std::views;

    auto hash = [&](int i) {
        String string(256);
        auto name = std::format("{}-{}", input, i);
        auto hash = string.hashcode2(name);
        return hash;
    };

    auto hashes = iota(0, 128) | transform(hash) | transform(hex2bin);
    return hashes;
}

int64_t hash_square_pop(const std::string &input) {
    using namespace std::views;

    auto square = hash_square(input);
    return std::ranges::fold_left(square | transform(popcnt), 0, std::plus{});
}

int64_t hash_regions(const std::string &input) {
    using Coord = std::pair<int,int>;

    std::vector<std::string> square;
    std::ranges::copy(hash_square(input), back_inserter(square));
    std::set<Coord> visited;
    std::stack<Coord> to_visit;

    int64_t count = 0;

    // count the number of regions...
    for (int y = 0; y < square.size(); ++y) {
        for (int x = 0; x < 128; ++x) {
            if ((square[y][x] == '1') && (!visited.contains({x,y}))) {
                ++count;
                to_visit.emplace(x,y);

                while (!to_visit.empty()) {
                    auto [x1,y1] = to_visit.top();
                    to_visit.pop();

                    if (!visited.contains({x1, y1}) && (square[y1][x1] == '1')) {
                        visited.emplace(x1, y1);

                        if (x1 > 0)
                            to_visit.emplace(x1 - 1, y1);
                        if (x1 < 127)
                            to_visit.emplace(x1 + 1, y1);
                        if (y1 > 0)
                            to_visit.emplace(x1, y1 - 1);
                        if (y1 < 127)
                            to_visit.emplace(x1, y1 + 1);
                    }
                }
            }
        }
    }
    return count;

}

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

    SECTION("Hash square population count computes correctly") {
        REQUIRE(hex2bin("a0c2017") == "1010000011000010000000010111");
        REQUIRE(popcnt(hex2bin("a0c2017")) == 9);
        REQUIRE(hash_square_pop("flqrgnkx") == 8108);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Hash square region count computes correctly") {
        REQUIRE(hash_regions("flqrgnkx") == 1242);
    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = hash_square_pop(puzzleInput);
    return n;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = hash_regions(puzzleInput);
    return n;
}
