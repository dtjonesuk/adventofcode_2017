#include <advent.h>

// external declarations
int64_t partOne();
int64_t partTwo();
const extern std::string day;

int main() {
    run<int64_t>(day, "One", partOne);
    run<int64_t>(day, "Two", partTwo);
}