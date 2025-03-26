#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

template<typename It>
auto count_elements(It begin, It end) {
    std::map<typename It::value_type, int> counts;
    for (It it = begin; it != end; ++it) {
        counts[*it]++;
    }
    return counts;
}

struct PuzzleInput {
    std::unordered_map<std::string, int> programs{};
    std::unordered_map<std::string, std::vector<std::string>> adjacency_list{};

    const std::regex program_re{R"((\w+) \((\d+)\))"};
    const std::regex stack_re{R"(([^,\s]+))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            std::smatch smatch;
            if (std::regex_search(line, smatch, program_re)) {
                std::string name = smatch[1];
                int weight = stoi(smatch[2]);

                programs.emplace(name, weight);
                if (smatch.suffix().str().starts_with(" -> ")) {
                    std::string children = smatch.suffix().str().substr(4);

                    for (auto it = std::sregex_iterator(children.begin(), children.end(), stack_re);
                         it != std::sregex_iterator(); ++it) {
                        const std::smatch &smatch = *it;
                        adjacency_list[name].push_back(smatch.str());
                    }
                }
            }
        }
    }

    std::string find_root() {
        std::set<std::string> nodes;
        for (const auto &[name, weight]: programs)
            nodes.insert(name);

        for (const auto &[name, children]: adjacency_list) {
            for (const auto &child: children) {
                nodes.erase(child);
            }
        }

        assert(nodes.size() == 1);
        return *nodes.begin();
    }

    int64_t find_weight(const std::string &program, int level = 0) {
//        std::cout << program << " weight is " << programs[program] << std::endl;
        int64_t total = programs[program];

        if (adjacency_list.contains(program)) {
            // find the weights of all children
            std::vector<int64_t> weights;
            const auto& children = adjacency_list[program];
            for (const auto &child: children) {
                auto w = find_weight(child, level + 1);
                weights.push_back(w);
                total += w;
            }

            auto counts = count_elements(weights.begin(), weights.end());
            if (counts.size() != 1) {
                auto mismatch_value = std::find_if(counts.begin(), counts.end(), [](auto p) { return p.second == 1; });
                auto mismatch_weight = std::find(weights.begin(), weights.end(), mismatch_value->first);
                auto child = children[std::distance(weights.begin(), mismatch_weight)];

                    auto normal = std::find_if(counts.begin(), counts.end(), [](auto p) { return p.second > 1; });
                    std::cout << level << ": " << child << " weight (" << mismatch_value->first << ") does not match " << normal->first
                              << " difference = " << std::abs(normal->first - mismatch_value->first) << std::endl;

            }
        }
        return total;
    }
};



/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.programs.size() == 13);
    REQUIRE(input.adjacency_list.size() == 4);

    SECTION("Find root node correctly") {
        REQUIRE(input.find_root() == "tknk");
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.programs.size() == 13);
    REQUIRE(input.adjacency_list.size() == 4);

    SECTION("") {

    }
}

#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    std::cout << input.find_root() << std::endl;
    return 0;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    std::string root = input.find_root();
    input.find_weight(root);
    return 0;
}
