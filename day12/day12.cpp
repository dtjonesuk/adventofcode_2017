#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

struct Graph {
    std::map<int, std::vector<int>> edges;

    int count_connected() {
        // keep a list of all visited
        std::set<int> visited;
        std::stack<int> to_visit;
        int count = 0;

        // traverse edges starting at 0
        to_visit.push(0);

        while (!to_visit.empty()) {
            // take top of stack
            int current = to_visit.top();
            to_visit.pop();

            if (visited.contains(current))
                continue;

            // mark as visited
            visited.insert(current);

            // increment count
            ++count;

            // add connected vertices to stack
            for (const int v: edges.at(current)) {
                if (!visited.contains(v))
                    to_visit.push(v);
            }
        }

        return count;
    }

    int count_groups() {
        // keep a list of all visited
        std::set<int> visited;
        std::stack<int> to_visit;
        int count = 0;

        // traverse edges starting at 0
        for (auto &edge: edges) {
            // skip any verts that have already been visited
            int current = edge.first;
            if (visited.contains(current))
                continue;

            // add this vert to  stack
            to_visit.push(current);

            // increment group count
            ++count;

            // now traverse all edges connected to this vert
            while (!to_visit.empty()) {
                // take top of stack
                current = to_visit.top();
                to_visit.pop();

                if (visited.contains(current))
                    continue;

                // mark as visited
                visited.insert(current);

                // add connected vertices to stack
                for (const int v: edges.at(current)) {
                    if (!visited.contains(v))
                        to_visit.push(v);
                }
            }

        }

        return count;
    }
};

struct PuzzleInput {
    Graph graph;

    const std::regex program_re{R"((\d+))"};
    const std::regex edges_re{R"(([^,\s]+))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            std::smatch smatch;
            if (std::regex_search(line, smatch, program_re)) {
                int name = stoi(smatch[1]);

                if (smatch.suffix().str().starts_with(" <-> ")) {
                    std::string connections = smatch.suffix().str().substr(5);

                    for (auto it = std::sregex_iterator(connections.begin(), connections.end(), edges_re);
                         it != std::sregex_iterator(); ++it) {
                        const std::smatch &smatch = *it;
                        graph.edges[name].push_back(stoi(smatch.str()));
                    }
                }
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

    REQUIRE(input.graph.edges.size() == 7);

    SECTION("Connected vertices count computes correctly") {
        REQUIRE(input.graph.count_connected() == 6);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.graph.edges.size() == 7);

    SECTION("Connected groups count computes correctly") {
        REQUIRE(input.graph.count_groups() == 2);
    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    int n = input.graph.count_connected();

    return n;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    int n = input.graph.count_groups();
    return n;
}
