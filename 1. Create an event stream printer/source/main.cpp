#include <sepia.hpp>

#include <iostream>

/// handleEvent is an event callback for an event stream observable.
void handleEvent(sepia::AtisEvent atisEvent) {
    if (atisEvent.isThresholdCrossing) {
        std::cout << "|";
    } else {
        std::cout << "-";
    }
    std::cout.flush();
}

/// handleException does nothing.
void handleException(std::exception_ptr) {}

int main() {
    auto eventStreamObservable = sepia::make_atisEventStreamObservable("/Users/Bob/Desktop/recording.es", handleEvent, handleException);

    for (;;) {}

    return 0;
}
