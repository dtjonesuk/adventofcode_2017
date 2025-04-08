#include <advent.h>


/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string testFilename = "test.txt";


struct Instruction {
    enum opcode {
        set,
        sub,
        mul,
        jnz
    } opcode;

    using Opcode = enum opcode;
    using Operand = std::variant<std::string, int64_t>;
    Operand left;
    Operand right;

    Instruction(const std::string &text, const Operand &left, const Operand &right = 0) : opcode(
            OpcodeFromString(text)),
                                                                                          left(left), right(right) {

    }


    static enum opcode OpcodeFromString(const std::string &text) {
        static const std::map<std::string, Opcode> opcodes = {
                {"set", set},
                {"sub", sub},
                {"mul", mul},
                {"jnz", jnz}
        };

        if (!opcodes.contains(text))
            std::cerr << "Invalid opcode: " << text << std::endl;

        return opcodes.at(text);
    }

    void print(std::ostream &ostream) {
        static const std::map<Opcode, std::string> opcodes = {
                {set, "set"},
                {sub, "sub"},
                {mul, "mul"},
                {jnz, "jnz"}
        };

        auto prn = [&](auto &operand) {
            ostream << operand;
        };

        ostream << opcodes.at(opcode) << " ";
        std::visit(prn, left);
        ostream << " ";
        std::visit(prn, right);

        ostream << std::endl;
    }
};

struct Interpreter {
    std::map<std::string, int64_t> registers;
    std::vector<Instruction> instructions;
    size_t pc = 0;
    int64_t frq = 0;
    size_t count = 0;

    explicit Interpreter(std::vector<Instruction> &&instructions) : instructions(std::move(instructions)) {}

    // return true if recovered otherwise false
    bool execute() {
        Instruction &instruction = instructions[pc++];
        switch (instruction.opcode) {
            case Instruction::set:
                set(instruction.left, instruction.right);
                break;
            case Instruction::sub:
                sub(instruction.left, instruction.right);
                break;
            case Instruction::mul:
                mul(instruction.left, instruction.right);
                break;
            case Instruction::jnz:
                jnz(instruction.left, instruction.right);
                break;
            default:
                throw std::runtime_error(std::format("Unknown opcode: {}", (int) instruction.opcode));
        }
        return pc < instructions.size();
    }

    void print() {
        for (const auto &[reg, value]: registers) {
            std::cout << reg << ": " << value << std::endl;
        }
        std::cout << "Frequency: " << frq << std::endl;
    }

private:

    int64_t &lvalue(Instruction::Operand &operand) {
        if (holds_alternative<std::string>(operand))
            return registers[get<std::string>(operand)];
        else
            return get<int64_t>(operand);
    }

    int64_t rvalue(Instruction::Operand &operand) {
        if (holds_alternative<std::string>(operand))
            return registers[get<std::string>(operand)];
        else
            return get<int64_t>(operand);
    }

    void set(Instruction::Operand &left, Instruction::Operand &right) {
        lvalue(left) = rvalue(right);
    }


    void add(Instruction::Operand &left, Instruction::Operand &right) {
        lvalue(left) += rvalue(right);
    }

    void sub(Instruction::Operand &left, Instruction::Operand &right) {
        lvalue(left) -= rvalue(right);
    }

    void mul(Instruction::Operand &left, Instruction::Operand &right) {
        ++count;
        lvalue(left) *= rvalue(right);
    }

    void mod(Instruction::Operand &left, Instruction::Operand &right) {
        lvalue(left) %= rvalue(right);
    }

    void snd(Instruction::Operand &left) {
        frq = lvalue(left);
    }

    bool rcv(Instruction::Operand &left) {
        if (lvalue(left) != 0) {
            return true;
        }
        return false;
    }

    void jgz(Instruction::Operand &left, Instruction::Operand &right) {
        if (lvalue(left) > 0)
            pc += lvalue(right) - 1;
    }

    void jnz(Instruction::Operand &left, Instruction::Operand &right) {
        if (lvalue(left) != 0)
            pc += lvalue(right) - 1;
    }

};

struct PuzzleInput {
    std::vector<Instruction> instructions{};

    std::regex instr1_re{R"((\w+) (\w)|(\-?\d+))"};
    std::regex instr2_re{R"((\w+) ((\w)|(\-?\d+)) ((\w)|(\-?\d+)))"};

    explicit PuzzleInput(std::istream &istream) {
        std::string line;

        while (std::getline(istream, line)) {
            // do something
            std::smatch smatch;
            if (std::regex_match(line, smatch, instr1_re)) {
                Instruction::Operand left;
                if (isalpha(smatch.str(2)[0]))
                    left = smatch.str(2);
                else
                    left = stoi(smatch.str(2));
                instructions.emplace_back(smatch.str(1), left);

            } else if (std::regex_match(line, smatch, instr2_re)) {
                Instruction::Operand left;
                Instruction::Operand right;
                if (isalpha(smatch.str(2)[0]))
                    left = smatch.str(2);
                else
                    left = stoi(smatch.str(2));

                if (isalpha(smatch.str(5)[0]))
                    right = smatch.str(5);
                else
                    right = stoi(smatch.str(5));

                instructions.emplace_back(smatch.str(1), left, right);
            } else {
                using namespace std::literals;
                auto s = "Could not parse instruction: {s}"s + line;
                throw std::runtime_error(s);
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

//    REQUIRE(input.lines.size() == 1);

    SECTION("") {

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
    Interpreter interpreter(std::move(input.instructions));

    while (interpreter.execute());

    return interpreter.count;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);
    int b = 79 * 100 + 100000;
    int c = b + 17000;

    int max = c;

    // Prime number sieve
    std::vector<bool> numbers(max + 1, true);

    // exclude 0 & 1 from result
    numbers[0] = false;
    numbers[1] = false;

    int n = 2;
    while (n <= max) {
        // eliminate all multiples of n
        for (int k = n * 2; k <= max; k += n) {
            numbers[k] = false;
        }
        // find next number
        n++;
        while (!numbers[n] && n <= max)
            ++n;
    }


    // Count all the composite numbers between b and c stepping by 17

    auto pipe = numbers | std::views::drop(b) | std::views::stride(17);
    auto count = std::ranges::count(pipe, false);

    return count;
}
