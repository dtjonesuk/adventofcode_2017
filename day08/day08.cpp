#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";

enum class Operator {
    Inc,
    Dec
};

const std::vector<std::function<int(int,int)>> operators{
    std::plus{},
    std::minus{}
};

Operator CreateOperator(const std::string &from) {
    if (from == "inc") {
        return Operator::Inc;
    } else if (from == "dec") {
        return Operator::Dec;
    } else {
        throw std::runtime_error(std::format("Unknown operator: {}", from));
    }
}

enum class Comparator {
    Less,
    LessEqual,
    Equal,
    NotEqual,
    Greater,
    GreaterEqual,
};

const std::vector<std::function<bool(int, int)>> comparators{
    std::less{},
    std::less_equal{},
    std::equal_to{},
    std::not_equal_to{},
    std::greater{},
    std::greater_equal{}
};

Comparator CreateComparator(const std::string &from) {
    if (from == "<") {
        return Comparator::Less;
    } else if (from == "<=") {
        return Comparator::LessEqual;
    } else if (from == "==") {
        return Comparator::Equal;
    } else if (from == "!=") {
        return Comparator::NotEqual;
    } else if (from == ">") {
        return Comparator::Greater;
    } else if (from == ">=") {
        return Comparator::GreaterEqual;
    } else {
        throw std::runtime_error(std::format("Unknown comparator: {}", from));
    }
}

struct Instruction {
    std::string left;
    Operator op;
    int immediate;
    Comparator cmp;
    std::string left;
    int right;

    Instruction(const std::string &reg, const std::string &op, int immediate, const std::string &cmp, const std::string &left,
                int right) :
            left(reg), op(CreateOperator(op)), immediate(immediate), cmp(CreateComparator(cmp)), left(left), right(right) {}
};

struct CPU {
    std::vector<Instruction> instructions;
    std::map<std::string, int> registers;
    int highest{};

    explicit CPU(std::vector<Instruction> &instructions) : instructions(std::move(instructions)) {}

    void run() {
        for (const Instruction& instruction:instructions) {
            execute(instruction);
        }
    }

private:
    void execute(const Instruction &instruction) {
        auto comparator = comparators[static_cast<size_t>(instruction.cmp)];
        int left = registers[instruction.left];
        if (comparator(left, instruction.right)) {
            auto op = operators[static_cast<size_t>(instruction.op)];
            int value = registers[instruction.left];
            value = op(value, instruction.immediate);
            registers[instruction.left] = value;

            if (value > highest)
                highest = value;
        }
    }
};
struct PuzzleInput {
    std::vector<Instruction> instructions;

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            std::stringstream ss{line};
            std::string str, reg, op, cmp, compared_reg;
            int immediate, compared_value;
            ss >> reg;
            ss >> op;
            ss >> immediate;
            ss >> str;
            ss >> compared_reg;
            ss >> cmp;
            ss >> compared_value;

            if (str != "if")
                throw std::runtime_error(std::format("Unknown instruction: {}", str));

            instructions.emplace_back(reg, op, immediate, cmp, compared_reg, compared_value);
//            std::cout << reg << op << immediate << compared_reg << cmp << compared_value << std::endl;
        }
    }

};

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.instructions.size() == 4);

    CPU cpu(input.instructions);

    SECTION("CPU executes instructions correctly") {
        cpu.run();

        REQUIRE(cpu.registers["a"] == 1);
        REQUIRE(cpu.registers["b"] == 0);
        REQUIRE(cpu.registers["c"] == -10);
    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(testFilename);
    PuzzleInput input(istream);

    REQUIRE(input.instructions.size() == 4);
    CPU cpu(input.instructions);

    SECTION("CPU executes instructions correctly") {
        cpu.run();
        REQUIRE(cpu.highest == 10);
    }
}

#endif

/// Part One Solution
int64_t partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    CPU cpu(input.instructions);
    cpu.run();

    auto comp = [](const auto &l, const auto &r){
            return std::less{}(l.second, r.second);
    };
    auto n = std::max_element(cpu.registers.begin(), cpu.registers.end(),
                              comp);
    return n->second;
}


/// Part Two Solution
int64_t partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);
    CPU cpu(input.instructions);
    cpu.run();

    return cpu.highest;
}
