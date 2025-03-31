#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct SpinMove {
    int count;
};

struct ExchangeMove {
    int from;
    int to;
};

struct PartnerMove {
    char from;
    char to;
};

using DanceMove = std::variant<SpinMove, ExchangeMove, PartnerMove>;

struct MoveVisitor {
    MoveVisitor(const std::string &input) : input(input) {}

    std::string input;

    void operator()(SpinMove &move) {
        for (int i = 0; i < move.count; ++i) {
            for (int n = input.size() - 1; n > 0; --n) {
                std::swap(input[n], input[n - 1]);
            }
        }
    }

    void operator()(ExchangeMove &move) {
        std::swap(input[move.from], input[move.to]);
    }

    void operator()(PartnerMove &move) {
        int from = -1;
        int to = -1;

        for (int i = 0; i < input.size(); ++i) {
            if (input[i] == move.from)
                from = i;
            else if (input[i] == move.to)
                to = i;
            if ((from >= 0) && (to >= 0)) {
                std::swap(input[from], input[to]);
                return;
            }
        }
    }

    void visit(DanceMove &move) {
        std::visit(*this, move);
    }
};

struct PuzzleInput {
    std::vector<DanceMove> moves;

    const std::regex spin_re{R"(s(\d+))"};
    const std::regex exch_re{R"(x(\d+)/(\d+))"};
    const std::regex part_re{R"(p(\w)/(\w))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line, ',')) {
            // do something
            std::smatch smatch;
            if (std::regex_match(line, smatch, spin_re)) {
                moves.push_back(SpinMove{stoi(smatch.str(1))});
            } else if (std::regex_match(line, smatch, exch_re)) {
                moves.push_back(ExchangeMove{stoi(smatch.str(1)), stoi(smatch.str(2))});
            } else if (std::regex_match(line, smatch, part_re)) {
                moves.push_back(PartnerMove{smatch.str(1)[0], smatch.str(2)[0]});
            } else {
                throw std::runtime_error("Unable to parse input: " + line);
            }
        }
    }

};

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.moves.size() == 3);

    SECTION("") {
        MoveVisitor visitor("abcde");

        visitor.visit(input.moves[0]);
        REQUIRE(visitor.input == "eabcd");

        visitor.visit(input.moves[1]);
        REQUIRE(visitor.input == "eabdc");

        visitor.visit(input.moves[2]);
        REQUIRE(visitor.input == "baedc");

    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);


    SECTION("Detect repeat in sequence") {
        MoveVisitor visitor("abcde");
        std::set<std::string> visited;

        for (int i = 0; i < 1'000'000'000; ++i) {
            for (DanceMove &move: input.moves) {
                visitor.visit(move);
            }
            if (visited.contains(visitor.input)) {
                std::cout << "Repeat found: " << i << std::endl;
                REQUIRE(i==4);
                break;
            }
            visited.insert(visitor.input);
        }
    }
}

#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    MoveVisitor visitor{"abcdefghijklmnop"};

    for (DanceMove &move: input.moves) {
        visitor.visit(move);
    }
    std::cout << visitor.input << std::endl;
    return 0;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    // determine point at which sequence repeats
    int64_t repeat{};
    {
        MoveVisitor visitor("abcdefghijklmnop");
        std::set<std::string> visited;

        for (int i = 0; i < 1'000'000'000; ++i) {
            for (DanceMove &move: input.moves) {
                visitor.visit(move);
            }
            if (visited.contains(visitor.input)) {
//                std::cout << "Repeat found: " << i << std::endl;
                repeat = i;
                break;
            }
            visited.insert(visitor.input);
        }
    }

    MoveVisitor visitor("abcdefghijklmnop");
    for (int i = 0; i < (1'000'000'000 % repeat); ++i) {
        for (DanceMove &move: input.moves) {
            visitor.visit(move);
        }
    }

    std::cout << visitor.input << std::endl;
    return 0;
}
