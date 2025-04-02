#include <advent.h>




/// Benchmarks a callable
template<typename T>
std::pair<double, T> eval(std::function<T(void)> func) {
    const auto t1 = std::chrono::high_resolution_clock::now();
    T result = func();
    const auto t2 = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> seconds = t2 - t1;

    return std::make_pair(seconds.count(), result);
}


template<typename T>
void run(const std::string &day, const std::string &part, std::function<T(void)> func) {
    std::cout << std::fixed << std::setprecision(3);
    auto [seconds, result] = eval<T>(func);
    auto get_string = [](auto v) {
        if constexpr (std::is_same_v<decltype(v), std::string>)
            return v;
        else
            return std::to_string(v);
    };
    std::cout << "Day " << day << ": Part " << part << " = " << std::visit(get_string, result) << "\t\t (completed in "
              << seconds
              << "s)." << std::endl;
}

int main() {
    run<ReturnType>(day, "One", partOne);
    run<ReturnType>(day, "Two", partTwo);
}