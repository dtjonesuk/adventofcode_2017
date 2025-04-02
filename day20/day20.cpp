#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

template<typename T>
struct Vector3 {
    Vector3(T x, T y, T z) : x(x), y(y), z(z) {}

    T x;
    T y;
    T z;

    // Returns the Manhattan distance from the origin
    [[nodiscard]] T distance() const {
        return std::abs(x) + std::abs(y) + std::abs(z);
    }

    bool operator==(const Vector3 &other) const {
        return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
    }

    bool operator<(const Vector3 &other) const {
        return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
    }
    
    Vector3 &operator+=(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
};

template<typename T>
struct Particle {
    Particle(Vector3<T> pos, Vector3<T> vel, Vector3<T> acc) : position(pos), velocity(vel), acceleration(acc) {}

    Vector3<T> position;
    Vector3<T> velocity;
    Vector3<T> acceleration;

    bool operator==(const Particle &other) const {
        return std::tie(position, velocity, acceleration) ==
               std::tie(other.position, other.velocity, other.acceleration);
    }
};

struct PuzzleInput {
    std::vector<Particle<int64_t>> particles;

    const std::regex input_re{R"((\w)=<\s*(-?\d+),\s*(-?\d+),\s*(-?\d+)>)"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            auto line_begin = std::sregex_iterator(line.begin(), line.end(), input_re);
            auto line_end = std::sregex_iterator();

            std::vector<Vector3<int64_t>> vectors;
            for (std::sregex_iterator i = line_begin; i != line_end; ++i) {
                const std::smatch &smatch = *i;
                vectors.emplace_back(stoi(smatch.str(2)), stoi(smatch.str(3)), stoi(smatch.str(4)));
            }
            if (vectors.size() != 3)
                throw std::runtime_error(std::format("Unable to parse particle: {}", line));

            // do something
            particles.emplace_back(vectors[0], vectors[1], vectors[2]);
        }
    }

};

template<typename T>
struct Simulation {
    Simulation(std::vector<Particle<T>> &&particles) : particles(std::move(particles)) {}

    void simulate(size_t n) {
        for (size_t i = 0; i < n; i++) {
            tick();
        }
    }

    void simulate2(size_t n) {
        for (size_t i = 0; i < n; i++) {
            tick();
            resolveCollisions();
        }
    }

    void resolveCollisions() {
        // sort particles by position
        std::ranges::sort(particles, std::less{}, [](const auto &particle) { return particle.position; });

        // find adjacent particles that have same position
        auto adj = particles |
                   std::views::transform(&Particle<int64_t>::position) |
                   std::views::adjacent<2> |
                   std::views::filter([](const auto &p) { return p.first == p.second; }) |
                   std::views::transform([](const auto &p) { return p.first; });

        // output adjacent particles to a set of locations
        std::set<Vector3<int64_t>> collisions(begin(adj), end(adj));
        
        // erase particles whose position is in the set of locations
        if (!collisions.empty())
            std::erase_if(particles, [&](const auto &p) { return collisions.contains(p.position); });
    }


    void tick() {
        for (Particle<T> &particle: particles) {
            particle.velocity += particle.acceleration;
            particle.position += particle.velocity;
        }
    }

    std::vector<Particle<T>> particles;
};
/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    SECTION("Input parses correctly") {
        REQUIRE(input.particles.size() == 2);
        REQUIRE(input.particles[0] == Particle<int64_t>({3, 0, 0}, {2, 0, 0}, {-1, 0, 0}));
        REQUIRE(input.particles[1] == Particle<int64_t>({4, 0, 0}, {0, 0, 0}, {-2, 0, 0}));
    }

    SECTION("Simulation tick calculates correctly") {
        Simulation sim(std::move(input.particles));
        
        sim.tick();
        REQUIRE(sim.particles[0].position == Vector3<int64_t>{4,0,0});
        REQUIRE(sim.particles[1].position == Vector3<int64_t>{2,0,0});
    }
    
    SECTION("") {
        Simulation sim(std::move(input.particles));
        sim.simulate(100);
        
        auto particle = std::ranges::min_element(sim.particles, std::less{}, [](const auto& part) {return part.position.distance(); });
        REQUIRE (std::distance(begin(sim.particles), particle) == 0);
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

    Simulation sim(std::move(input.particles));
    sim.simulate(500);

    auto particle = std::ranges::min_element(sim.particles, std::less{},
                                             [](const auto &part) { return part.position.distance(); });
    auto n = std::distance(begin(sim.particles), particle);

    return n;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Simulation sim(std::move(input.particles));
    sim.simulate2(500);


    return sim.particles.size();
}
