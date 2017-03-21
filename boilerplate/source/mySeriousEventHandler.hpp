#pragma once

#include <utility>
#include <cmath>
#include <cstdint>

/// MySeriousEventHandler rotate the scene with the given angular speed.
template <typename Event, std::size_t width, std::size_t height, typename HandleEvent>
class MySeriousEventHandler {
    public:
        MySeriousEventHandler(const double& angularSpeed, HandleEvent handleEvent) :
            _angularSpeed(angularSpeed),
            _handleEvent(std::forward<HandleEvent>(handleEvent)),
            _angle(0.0),
            _previousTimestamp(0)
        {
        }
        MySeriousEventHandler(const MySeriousEventHandler&) = delete;
        MySeriousEventHandler(MySeriousEventHandler&&) = default;
        MySeriousEventHandler& operator=(const MySeriousEventHandler&) = delete;
        MySeriousEventHandler& operator=(MySeriousEventHandler&&) = default;
        virtual ~MySeriousEventHandler() {}

        /// operator() handles an event.
        virtual void operator()(Event event) {
            if (event.timestamp >= _previousTimestamp) {
                _angle += _angularSpeed * (event.timestamp - _previousTimestamp);
            }
            _previousTimestamp = event.timestamp;
            const auto angleCosine = std::cos(_angle);
            const auto angleSine = std::sin(_angle);
            const auto xCenter = static_cast<double>(width) / 2.0;
            const auto yCenter = static_cast<double>(height) / 2.0;
            const auto xDelta = event.x - xCenter;
            const auto yDelta = event.y - yCenter;
            const auto x = xCenter + xDelta * angleCosine - yDelta * angleSine;
            const auto y = xCenter + xDelta * angleSine + yDelta * angleCosine;
            if (x < width && x >= 0 && y < height && y >= 0) {
                event.x = static_cast<std::size_t>(x);
                event.y = static_cast<std::size_t>(y);
                _handleEvent(std::move(event));
            }
        }

    protected:
        const double _angularSpeed;
        HandleEvent _handleEvent;
        double _angle;
        uint64_t _previousTimestamp;
};

/// make_mySeriousEventHandler creates a MySeriousEventHandler from a functor.
template<typename Event, std::size_t width, std::size_t height, typename HandleEvent>
MySeriousEventHandler<Event, width, height, HandleEvent> make_mySeriousEventHandler(
    const double& angularSpeed,
    HandleEvent handleEvent
) {
    return MySeriousEventHandler<Event, width, height, HandleEvent>(
        angularSpeed,
        std::forward<HandleEvent>(handleEvent)
    );
}
