#pragma once

#include "../third_party/sepia/source/sepia.hpp"
#include "../third_party/tarsier/source/mirror_x.hpp"

#include <cstdint>
#include <utility>

/// wrapper holds a reference to a buffer.
/// It can be used as an event handler for a tarsier partial handler.
class wrapper {
    public:
    wrapper(std::vector<sepia::dvs_event>& output_buffer) : _output_buffer(output_buffer) {}
    wrapper(const wrapper&) = delete;
    wrapper(wrapper&&) = default;
    wrapper& operator=(const wrapper&) = delete;
    wrapper& operator=(wrapper&&) = default;
    virtual ~wrapper() {}

    /// operator() handles an event.
    virtual void operator()(sepia::dvs_event event) {
        _output_buffer.push_back(event);
    }

    protected:
    std::vector<sepia::dvs_event>& _output_buffer;
};

/// buffered_wrapper inverts the x coordinate.
/// The protected tarsier event handler member needs to be fully defined,
/// hence the use of a dedicated 'wrapper' class instead of a lambda function.
class buffered_wrapper {
    public:
    buffered_wrapper(uint16_t width) : _output_buffer(), _handle_event(width, wrapper(_output_buffer)) {}
    buffered_wrapper(const buffered_wrapper&) = delete;
    buffered_wrapper(buffered_wrapper&&) = default;
    buffered_wrapper& operator=(const buffered_wrapper&) = delete;
    buffered_wrapper& operator=(buffered_wrapper&&) = default;
    virtual ~buffered_wrapper() {}

    /// input_to_output handles a buffer, and returns an output buffer.
    virtual const std::vector<sepia::dvs_event>& input_to_output(const std::vector<sepia::dvs_event>& buffer) {
        _output_buffer.clear();
        _output_buffer.reserve(buffer.size());
        for (const auto event : buffer) {
            _handle_event(event);
        }
        return _output_buffer;
    }

    protected:
    std::vector<sepia::dvs_event> _output_buffer;
    tarsier::mirror_x<sepia::dvs_event, wrapper> _handle_event; // fully specialized event handler
};
