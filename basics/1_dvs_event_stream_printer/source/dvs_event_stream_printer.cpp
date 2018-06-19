#include "../third_party/sepia/source/sepia.hpp"
#include <iostream>

/// filename points to the Event Stream file to read.
/// The use of the '__FILE__' macro and the 'sepia::dirname' and 'sepia::join' functions
/// allows to resolve the path relatively to the source file, rather than to the compiled executable.
const auto filename = sepia::join(
    {sepia::dirname(SEPIA_DIRNAME), "third_party", "sepia", "third_party", "event_stream", "examples", "dvs.es"});

/// handle_event is an event callback for a DVS Event Stream observable.
void handle_event(sepia::dvs_event dvs_event) {
    if (dvs_event.is_increase) {
        std::cout << "+";
    } else {
        std::cout << "-";
    }
    std::cout.flush();
}

int main() {
    sepia::join_observable<sepia::type::dvs>(sepia::filename_to_ifstream(filename), handle_event);
    return 0;
}
