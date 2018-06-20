#pragma once

#include <atomic>
#include <cstdint>
#include <utility>
#include <vector>

/// tarsier is a collection of event handlers.
namespace tarsier {

    /// mask_isolated propagates only events that are not isolated spatially or temporally.
    template <typename Event, typename HandleEvent>
    class mask_isolated {
        public:
        mask_isolated(
            uint16_t width,
            uint16_t height,
            std::atomic<uint64_t>& temporal_window,
            HandleEvent handle_event) :
            _width(width),
            _height(height),
            _temporal_window(temporal_window),
            _handle_event(std::forward<HandleEvent>(handle_event)),
            _ts(width * height, 0) {}
        mask_isolated(const mask_isolated&) = delete;
        mask_isolated(mask_isolated&&) = default;
        mask_isolated& operator=(const mask_isolated&) = delete;
        mask_isolated& operator=(mask_isolated&&) = default;
        virtual ~mask_isolated() {}

        /// operator() handles an event.
        virtual void operator()(Event event) {
            const auto index = event.x + event.y * _width;
            _ts[index] = event.t + _temporal_window.load(std::memory_order_relaxed);
            if ((event.x > 0 && _ts[index - 1] > event.t) || (event.x < _width - 1 && _ts[index + 1] > event.t)
                || (event.y > 0 && _ts[index - _width] > event.t)
                || (event.y < _height - 1 && _ts[index + _width] > event.t)) {
                _handle_event(event);
            }
        }

        protected:
        const uint16_t _width;
        const uint16_t _height;
        std::atomic<uint64_t>& _temporal_window;
        HandleEvent _handle_event;
        std::vector<uint64_t> _ts;
    };

    /// make_mask_isolated creates a mask_isolated from a functor.
    template <typename Event, typename HandleEvent>
    mask_isolated<Event, HandleEvent> make_mask_isolated(
        uint16_t width,
        uint16_t height,
        std::atomic<uint64_t>& temporal_window,
        HandleEvent handle_event) {
        return mask_isolated<Event, HandleEvent>(
            width, height, temporal_window, std::forward<HandleEvent>(handle_event));
    }
}
