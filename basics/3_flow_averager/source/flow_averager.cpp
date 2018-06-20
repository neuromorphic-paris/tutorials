#include "../third_party/sepia/source/sepia.hpp"
#include "../third_party/tarsier/source/compute_flow.hpp"
#include <iostream>

/// filename points to the Event Stream file to read.
const auto filename = sepia::join(
    {sepia::dirname(SEPIA_DIRNAME), "third_party", "sepia", "third_party", "event_stream", "examples", "dvs.es"});

/// flow_event contains the parameters of a flow update.
struct flow_event {
    uint64_t t;
    float vx;
    float vy;
} __attribute__((packed));

int main() {
    const auto header = sepia::read_header(sepia::filename_to_ifstream(filename));

    float vx_sum = 0;
    float vy_sum = 0;
    uint64_t previous_t = 0;
    std::size_t count = 0;

    sepia::join_observable<sepia::type::dvs>(
        sepia::filename_to_ifstream(filename),
        sepia::make_split<sepia::type::dvs>(
            tarsier::make_compute_flow<sepia::simple_event, flow_event>(
                header.width,
                header.height,
                2,   // spatial window's radius
                1e6, // temporal window
                10,  // minimum number of events in the spatio-temporal window required to trigger a flow event
                [](sepia::simple_event simple_event, float vx, float vy) -> flow_event {
                    return {simple_event.t, vx, vy};
                },
                [&](flow_event flow_event) {
                    ++count;
                    vx_sum += flow_event.vx;
                    vy_sum += flow_event.vy;
                    if (flow_event.t >= previous_t + 100000) {
                        std::cout << "average flow: (" << (flow_event.vx * 1e6) << ", " << (flow_event.vy * 1e6)
                                  << ") pixels / second" << std::endl;
                        previous_t = flow_event.t;
                        count = 0;
                    }
                }),
            [](sepia::simple_event) {}));
    return 0;
}
