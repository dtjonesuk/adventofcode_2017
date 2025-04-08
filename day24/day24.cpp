#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct PuzzleInput {
    std::vector<std::pair<int, int>> ports;
    const std::regex input_re{R"((\d+)\/(\d+))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            std::smatch smatch;
            if (std::regex_match(line, smatch, input_re)) {
                int a = stoi(smatch.str(1));
                int b = stoi(smatch.str(2));
                ports.emplace_back(a, b);
            }
        }
    }

};

std::pair<int, int> longest_bridge(const std::vector<std::pair<int, int>> &ports, const int previous) {
    int strongest = 0;
    int longest = 0;

    for (const auto port: ports) {
        if ((port.first == previous) || (port.second == previous)) {

            int bridge = port.first + port.second;

            std::vector<std::pair<int, int>> remainder;
            std::ranges::remove_copy(ports, std::back_inserter(remainder), port);

            auto [length, strength] = longest_bridge(remainder, bridge - previous);
            length++;
            strength += bridge;

            if (length > longest) {
                longest = length;
                strongest = strength;
            } else if (length == longest) {
                if (strength > strongest) {
                    strongest = strength;
                }
            }

        }
    }
    return {longest, strongest};
}

std::pair<int, int> longest_bridge(const std::vector<std::pair<int, int>> &ports) {
    int n = 0;
    int strongest = 0;
    int longest = 0;

    for (const auto port: ports | std::views::filter([=](const auto &p) { return p.first == n || p.second == n; })) {
        int bridge = port.first + port.second;

        std::vector<std::pair<int, int>> remainder;
        std::ranges::remove_copy(ports, std::back_inserter(remainder), port);

        auto [length, strength] = longest_bridge(remainder, bridge);
        length++;
        strength += bridge;

        if (length > longest) {
            longest = length;
            strongest = strength;
        } else if (length == longest) {
            if (strength > strongest) {
                strongest = strength;
            }
        }
    }
    return {longest, strongest};
}

int strongest_bridge(const std::vector<std::pair<int, int>> &ports, const int previous) {
    int strongest = 0;

    for (const auto port: ports) {
        if ((port.first == previous) || (port.second == previous)) {

            int bridge = port.first + port.second;

            std::vector<std::pair<int, int>> remainder;
            std::ranges::remove_copy(ports, std::back_inserter(remainder), port);

            strongest = std::max(bridge + strongest_bridge(remainder, bridge - previous), strongest);
        }
    }
    return strongest;
}

int strongest_bridge(const std::vector<std::pair<int, int>> &ports) {
    int n = 0;
    int strongest = 0;
    for (const auto port: ports | std::views::filter([=](const auto &p) { return p.first == n || p.second == n; })) {
        int bridge = port.first + port.second;

        std::vector<std::pair<int, int>> remainder;
        std::ranges::remove_copy(ports, std::back_inserter(remainder), port);

        strongest = std::max(bridge + strongest_bridge(remainder, bridge), strongest);
    }
    return strongest;
}

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.ports.size() == 8);

    SECTION("") {
        REQUIRE(strongest_bridge(input.ports) == 31);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("") {
        auto [length, strength] = longest_bridge(input.ports);
        REQUIRE(length == 4);
        REQUIRE(strength == 19);
    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto n = strongest_bridge(input.ports);
    return n;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    auto [length, strength] = longest_bridge(input.ports);
    return strength;
}
