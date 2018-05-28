#include "../third_party/sepia/source/sepia.hpp"
#include "../third_party/tarsier/source/compute_flow.hpp"
#include <iostream>

/// filename points to the Event Stream file to read.
const auto filename = sepia::join({sepia::dirname(sepia::dirname(__FILE__)),
                                   "third_party",
                                   "sepia",
                                   "third_party",
                                   "event_stream",
                                   "examples",
                                   "dvs.es"});

/// flow_event contains the parameters of a flow update.
struct flow_event {
    uint64_t t;
    uint16_t x;
    uint16_t y;
    float vx;
    float vy;
} __attribute__((packed));

int main() {
    // read the header
    const auto header = sepia::read_header(sepia::filename_to_ifstream(filename));

    // declare variables to store a state
    float vx_sum = 0;
    float vy_sum = 0;
    uint64_t previous_t = 0;
    std::size_t count = 0;

    // create the event handling pipeline
    auto observable = sepia::make_observable<sepia::type::dvs>(
        sepia::filename_to_ifstream(filename),
        sepia::make_split<sepia::type::dvs>(
            tarsier::make_compute_flow<sepia::simple_event, flow_event>(
                header.width,
                header.height,
                2,    // spatial window's radius
                1e6,  // temporal window
                10,   // minimum number of events in the spatio-temporal window required to trigger a flow event
                1e-2, // maximum flow speed in pixels per microseconds over which flow events are not triggered
                [&](sepia::simple_event simple_event, float vx, float vy) -> flow_event {
                    return {simple_event.t, simple_event.x, simple_event.y, vx, vy};
                },
                [&](flow_event flow_event) {
                    ++count;
                    vx_sum += flow_event.vx;
                    vy_sum += flow_event.vy;
                    if (flow_event.t >= previous_t + 100000) {
                        std::cout << "Average flow: (" << (flow_event.vx * 1e6) << ", " << (flow_event.vy * 1e6)
                                  << ") pixels / second" << std::endl;
                        previous_t = flow_event.t;
                        count = 0;
                    }
                }),
            [](sepia::simple_event) {}),
        [](std::exception_ptr) -> void {});

    // loop forever
    for (;;) {
    }
    return 0;
}
