#include "buffered_wrapper.hpp"
#include <iostream>

/// filename points to the Event Stream file to read.
const auto filename = sepia::join(
    {sepia::dirname(SEPIA_DIRNAME), "third_party", "sepia", "third_party", "event_stream", "examples", "dvs.es"});

int main(int argc, char* argv[]) {
    // read the header
    const auto header = sepia::read_header(sepia::filename_to_ifstream(filename));

    // build event buffers
    std::size_t number_of_events = 0;
    std::vector<std::vector<sepia::dvs_event>> buffers{{}};
    sepia::join_observable<sepia::type::dvs>(sepia::filename_to_ifstream(filename), [&](sepia::dvs_event dvs_event) {
        ++number_of_events;
        if (buffers.back().size() >= 1000) {
            buffers.emplace_back();
        }
        buffers.back().push_back(dvs_event);
    });

    // create a buffered wrapper
    buffered_wrapper mirror_x_wrapper(header.width);

    // loop over the buffers
    // calculate the average x coordinate and the average y coordinate
    // for both mirrored and non-mirrored events
    auto mean_x = 0.0;
    auto mean_y = 0.0;
    auto mean_mirrored_x = 0.0;
    auto mean_mirrored_y = 0.0;
    for (const auto& buffer : buffers) {
        for (const auto event : buffer) {
            mean_x += static_cast<double>(event.x) / number_of_events;
            mean_y += static_cast<double>(event.y) / number_of_events;
        }
        const auto output_buffer = mirror_x_wrapper.input_to_output(buffer);
        for (const auto event : output_buffer) {
            mean_mirrored_x += static_cast<double>(event.x) / number_of_events;
            mean_mirrored_y += static_cast<double>(event.y) / number_of_events;
        }
    }

    std::cout << "non-mirrored means: (" << mean_x << ", " << mean_y << ")\nmirrored means: (" << mean_mirrored_x
              << ", " << mean_mirrored_y << ")" << std::endl;
    return 0;
}
