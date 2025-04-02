#include <advent.h>

/// Change this to the current day
extern const std::string day = PROJECT_DAY;
static const std::string inputFilename = "input.txt";
static const std::string test1Filename = "test1.txt";
static const std::string test2Filename = "test2.txt";

struct Instruction {
    enum opcode {
        set,
        add,
        mul,
        mod,
        snd,
        rcv,
        jgz
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
                {"add", add},
                {"mul", mul},
                {"mod", mod},
                {"snd", snd},
                {"rcv", rcv},
                {"jgz", jgz}
        };

        if (!opcodes.contains(text))
            std::cerr << "Invalid opcode: " << text << std::endl;

        return opcodes.at(text);
    }

    void print(std::ostream &ostream) {
        static const std::map<Opcode, std::string> opcodes = {
                {set, "set"},
                {add, "add"},
                {mul, "mul"},
                {mod, "mod"},
                {snd, "snd"},
                {rcv, "rcv"},
                {jgz, "jgz"}
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

    explicit Interpreter(std::vector<Instruction> &&instructions) : instructions(std::move(instructions)) {}

    // return true if recovered otherwise false
    bool execute() {
        Instruction &instruction = instructions[pc++];
        switch (instruction.opcode) {
            case Instruction::set:
                set(instruction.left, instruction.right);
                break;
            case Instruction::add:
                add(instruction.left, instruction.right);
                break;
            case Instruction::mul:
                mul(instruction.left, instruction.right);
                break;
            case Instruction::mod:
                mod(instruction.left, instruction.right);
                break;
            case Instruction::snd:
                snd(instruction.left);
                break;
            case Instruction::rcv:
                return rcv(instruction.left);
                break;
            case Instruction::jgz:
                jgz(instruction.left, instruction.right);
                break;
            default:
                throw std::runtime_error(std::format("Unknown opcode: {}", (int) instruction.opcode));
        }
        return false;
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

    void mul(Instruction::Operand &left, Instruction::Operand &right) {
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

};

struct Pipe {
    std::queue<int64_t> &in;
    std::queue<int64_t> &out;
};

struct Interpreter2 {
    std::map<std::string, int64_t> registers;
    std::vector<Instruction> instructions;
    Pipe pipe;
    size_t pc = 0;
    size_t snd_count = 0;

    explicit Interpreter2(std::vector<Instruction> &instructions, Pipe pipe, int id) : instructions(instructions),
                                                                                       pipe(pipe) {
        registers["p"] = id;
    }

    // return true if waiting otherwise false
    bool execute() {
        bool waiting = false;
        Instruction &instruction = instructions[pc++];
        switch (instruction.opcode) {
            case Instruction::set:
                set(instruction.left, instruction.right);
                break;
            case Instruction::add:
                add(instruction.left, instruction.right);
                break;
            case Instruction::mul:
                mul(instruction.left, instruction.right);
                break;
            case Instruction::mod:
                mod(instruction.left, instruction.right);
                break;
            case Instruction::snd:
                snd(instruction.left);
                break;
            case Instruction::rcv:
                waiting = rcv(instruction.left);
                if (waiting)
                    pc--;
                return waiting;
                break;
            case Instruction::jgz:
                jgz(instruction.left, instruction.right);
                break;
            default:
                throw std::runtime_error(std::format("Unknown opcode: {}", (int) instruction.opcode));
        }
        return false;
    }

    void print() {
        for (const auto &[reg, value]: registers) {
            std::cout << reg << ": " << value << std::endl;
        }
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

    void mul(Instruction::Operand &left, Instruction::Operand &right) {
        lvalue(left) *= rvalue(right);
    }

    void mod(Instruction::Operand &left, Instruction::Operand &right) {
        lvalue(left) %= rvalue(right);
    }

    void snd(Instruction::Operand &left) {
        pipe.out.push(rvalue(left));
        ++snd_count;
    }

    bool rcv(Instruction::Operand &left) {
        if (pipe.in.empty())
            return true;
        lvalue(left) = pipe.in.front();
        pipe.in.pop();
        return false;
    }

    void jgz(Instruction::Operand &left, Instruction::Operand &right) {
        if (rvalue(left) > 0)
            pc += rvalue(right) - 1;
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
                throw std::runtime_error(std::format("Could not parse instruction: {}", line));
            }
        }
    }

};

/// Unit tests
#ifdef CATCH2

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Part One Tests") {
    std::ifstream istream(test1Filename);
    PuzzleInput input(istream);

    REQUIRE(input.instructions.size() == 10);

    SECTION("Interpreter Part One") {
        Interpreter interpreter(std::move(input.instructions));
        CHECK(input.instructions.empty());

        while (!interpreter.execute()) {
        }

        REQUIRE(interpreter.frq == 4);

    }
}

TEST_CASE("Part Two tests") {
    std::ifstream istream(test2Filename);
    PuzzleInput input(istream);


    SECTION("Interpreter Part Two") {
        // construct interpreters with crossed-over i/o queues
        std::queue<int64_t> a, b;
        Interpreter2 inta(input.instructions, {a, b}, 0);
        Interpreter2 intb(input.instructions, {b, a}, 1);

        bool waiting_a = false;
        bool waiting_b = false;
        while (!(waiting_a && waiting_b)) {
            waiting_a = inta.execute();
            waiting_b = intb.execute();
        }

        std::map<std::string, int64_t> answers_a{
                {"a", 1},
                {"b", 2},
                {"c", 1},
                {"p", 0}
        };
        std::map<std::string, int64_t> answers_b{
                {"a", 1},
                {"b", 2},
                {"c", 0},
                {"p", 1}
        };

        REQUIRE(std::ranges::equal(inta.registers, answers_a));
        REQUIRE(std::ranges::equal(intb.registers, answers_b));
    }
}

#endif

/// Part One Solution
ReturnType partOne() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    Interpreter interpreter(std::move(input.instructions));

    while (!interpreter.execute()) {
    }

    return interpreter.frq;
}


/// Part Two Solution
ReturnType partTwo() {
    std::ifstream istream(inputFilename);
    PuzzleInput input(istream);

    std::queue<int64_t> a, b;
    Interpreter2 inta(input.instructions, {a, b}, 0);
    Interpreter2 intb(input.instructions, {b, a}, 1);

    bool waiting_a = false;
    bool waiting_b = false;
    while (!(waiting_a && waiting_b)) {
        waiting_a = inta.execute();
        waiting_b = intb.execute();
    }

    return intb.snd_count;
}
