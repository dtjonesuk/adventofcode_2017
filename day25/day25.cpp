#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";


struct Transition {
    enum Direction {
        Left,
        Right
    };

    int write;
    Direction move;
    char next_state;
};

struct TransitionFunction {
    TransitionFunction(Transition initial[2]) : transitions({initial[0], initial[1]}) {}

    std::array<Transition, 2> transitions;

    const Transition &operator()(int value) {
        return transitions[value];
    }
};

struct TuringMachine {
    std::set<char> states{};
    std::map<char, TransitionFunction> transition_functions{};
    std::deque<int> tape;

    char initial_state;
    int checksum_after;


    void execute() {
        int i = 0;
        current_state = initial_state;
        tape.push_back(0);
        current_position = begin(tape);
        while (i < checksum_after) {
            step();
            ++i;
        }
    }
    
private:
    std::deque<int>::iterator current_position;
    char current_state;


    void step() {
        const auto &delta = transition_functions.at(current_state)(*current_position);
        // Write value to current position
        *current_position = delta.write;
        
        // Set next state
        current_state = delta.next_state;
        
        if (delta.move == Transition::Right) {
            // Add a zero if at end of tape, otherwise move right 
            if (next(current_position) == end(tape)) {
                tape.push_back(0);
                current_position = prev(end(tape));
            } else {
                current_position++;
            }            
        } else {
            // Add a zero if at beginning of tape, otherwise move left
            if (current_position == begin(tape)) {
                tape.push_front(0);
                current_position = begin(tape);
            } else {
                --current_position;
            }
        }

    }
};


struct PuzzleInput {
    const std::regex begin_re{R"(Begin in state (\w).)"};
    const std::regex checksum_re{R"(Perform a diagnostic checksum after (\d+) steps.)"};
    const std::regex state_re{R"(In state (\w):)"};
    const std::regex current_re{R"(\s*If the current value is (\d):)"};
    const std::regex write_re{R"(\s*- Write the value (\d).)"};
    const std::regex move_re{R"(\s*- Move one slot to the (\w+).)"};
    const std::regex continue_re{R"(\s*- Continue with state (\w).)"};

    TuringMachine tm;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;


        while (std::getline(istream, line)) {
                                std::smatch smatch;
                        if (std::regex_match(line, smatch, begin_re)) {
                            tm.initial_state = smatch.str(1)[0];
                        } else if (std::regex_match(line, smatch, checksum_re)) {
                            tm.checksum_after = stoi(smatch.str(1));
                        } else if (std::regex_match(line, smatch, state_re)) {
                            char state = smatch.str(1)[0];
                            tm.states.insert(state);
                            Transition transitions[2], *transition{nullptr};

                            while (std::getline(istream, line) && !line.empty()) {
                                if (std::regex_match(line, smatch, current_re)) {
                                    int value = stoi(smatch.str(1));
                                    transition = &transitions[value];
                                } else if (std::regex_match(line, smatch, write_re)) {
                                    assert(transition != nullptr);
                                    transition->write = stoi(smatch.str(1));
                                } else if (std::regex_match(line, smatch, move_re)) {
                                    assert(transition != nullptr);
                                    transition->move = smatch.str(1) == "right" ? Transition::Direction::Right
                                                                                : Transition::Direction::Left;
                                } else if (std::regex_match(line, smatch, continue_re)) {
                                    assert(transition != nullptr);
                                    transition->next_state = smatch.str(1)[0];
                    }
                }

                tm.transition_functions.emplace(state, transitions);
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
    TuringMachine &tm = input.tm;
    
    REQUIRE(tm.states.size() == 2);
    REQUIRE(tm.transition_functions.size() == 2);
    REQUIRE(tm.initial_state == 'A');
    REQUIRE(tm.checksum_after == 6);

    SECTION("") {
        tm.execute();
        REQUIRE(std::ranges::count(tm.tape, 1) == 3);
        
//        for (const int n : tm.tape) {
//            std::cout << n << " ";
//        }
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
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);
    TuringMachine &tm = input.tm;
    
    tm.execute();
    auto n = std::ranges::count(tm.tape, 1);
    return n;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    return 0;
}
