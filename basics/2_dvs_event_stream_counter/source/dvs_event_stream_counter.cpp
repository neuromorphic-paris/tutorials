#include "../third_party/sepia/source/sepia.hpp"
#include <iostream>

/// filename points to the Event Stream file to read.
const auto filename = sepia::join({sepia::dirname(sepia::dirname(__FILE__)),
                                   "third_party",
                                   "sepia",
                                   "third_party",
                                   "event_stream",
                                   "examples",
                                   "dvs.es"});

int main() {
    std::size_t count = 0;
    uint64_t previous_t = 0;
    auto observable = sepia::make_observable<sepia::type::dvs>(
        sepia::filename_to_ifstream(filename),
        [&](sepia::dvs_event dvs_event) {
            ++count;
            if (dvs_event.t >= previous_t + 100000) {
                std::cout << (count * 10) << " events / second" << std::endl;
                previous_t = dvs_event.t;
                count = 0;
            }
        },
        [](std::exception_ptr) -> void {});
    for (;;) {
    }
    return 0;
}
