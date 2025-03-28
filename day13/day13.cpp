#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct Layer {
    explicit Layer(int size = 0) : depth(size) {}

    int depth{};
    int position{};
    int direction{1};
};

struct Firewall {
    explicit Firewall(std::map<int, int> &layer_map) {
        auto len = layer_map.crbegin()->first;
        layers.resize(len + 1);
        for (const auto &p: layer_map) {
            layers[p.first] = Layer(p.second);
        }
    }

    int current{};
    std::vector<Layer> layers;

    int traverse(int delay = 0) {
        int severity = 0;

        for (int i = 0; i < delay; ++i)
            tick();

        for (current = 0; current < layers.size(); ++current) {
            severity += tick();
        }
        return severity;
    }

    int tick() {
        int severity{};

        // is scanner at top of layer?
        if (layers[current].position == 0) {
            // caught
            severity = layers[current].depth * current;
        }

        // move all layers
        for (auto &layer: layers) {
            // only move layers with more than possible position
            if (layer.depth > 1) {
                int &pos = layer.position;
                pos += layer.direction;

                // 'bounce' off the ends of the layer
                if (pos == -1) {
                    pos = 1;
                    layer.direction = 1;
                } else if (pos == layer.depth) {
                    pos = layer.depth - 2;
                    layer.direction = -1;
                }
            }
        }

        return severity;
    }
};

struct PuzzleInput {
    std::map<int, int> layers;
    const std::regex input_re{R"((\d+): (\d+))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            std::smatch smatch;
            if (std::regex_match(line, smatch, input_re)) {
                layers.insert({stoi(smatch.str(1)), stoi(smatch.str(2))});
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

    REQUIRE(input.layers.size() == 4);

    SECTION("") {
        Firewall firewall(input.layers);
        REQUIRE(firewall.traverse() == 24);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.layers.size() == 4);

    SECTION("") {
        Firewall firewall(input.layers);
        REQUIRE(firewall.traverse(10) == 0);
    }
}

#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Firewall firewall(input.layers);

    return firewall.traverse();
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    int delay = 10;
    /// Need chinese remainder theorem?
//    int severity;
//    do {
//        Firewall firewall(input.layers);
//        severity = firewall.traverse(delay++);
//        std::cout << "\rdelay: " << delay << "\tseverity: " << severity << std::flush;
//    } while ( severity != 0);


    return delay - 1;
}
