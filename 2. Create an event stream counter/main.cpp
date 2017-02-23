#include <sepia.hpp>

#include <iostream>

int main() {
    std::size_t count = 0;
    uint64_t lastTimestamp = 0;

    auto eventStreamObservable = sepia::make_eventStreamObservable(
        "/Users/Bob/Desktop/recording.es",
        [count, lastTimestamp](sepia::Event event) mutable -> void {
            ++count;
            if (event.timestamp >= lastTimestamp + 100000) {
                std::cout << (count * 10) << " events / second" << std::endl;
                lastTimestamp = event.timestamp;
                count = 0;
            }
        },
        [](std::exception_ptr) -> void {}
    );

    for (;;) {}

    return 0;
}
