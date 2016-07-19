#include <opalKellyAtisSepia.hpp>

#include <iostream>

int main() {
    std::size_t count = 0;
    int64_t lastTimestamp = 0;

    auto camera = opalKellyAtisSepia::make_camera(
        [count, lastTimestamp](sepia::Event event) mutable -> void {
            ++count;
            if (event.timestamp >= lastTimestamp + 1000000) {
                std::cout << count << " events / second" << std::endl;
                lastTimestamp = event.timestamp;
                count = 0;
            }
        },
        [](std::exception_ptr) -> void {}
    );

    for (;;) {}

    return 0;
}
