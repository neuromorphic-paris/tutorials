#include <opalKellyAtisSepia.hpp>

#include <iostream>

/// handleEvent is an event callback for a camera.
void handleEvent(sepia::Event event) {
    if (event.isExposureMeasurement) {
        std::cout << "|";
    } else {
        std::cout << "-";
    }
    std::cout.flush();
}

/// handleException does nothing
void handleException(std::exception_ptr) {}

int main() {
    auto camera = opalKellyAtisSepia::make_camera(handleEvent, handleException);

    for (;;) {}

    return 0;
}
