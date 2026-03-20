// src/utils/timer.hpp
#pragma once
#include <chrono>
#include <cstdint>

namespace utils {

    inline int64_t now_ns() {
        return std::chrono::high_resolution_clock::now()
                .time_since_epoch().count();
    }

    inline int64_t now_ms() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
    }

    struct Latency {
        int64_t start_ns;

        Latency() : start_ns(now_ns()) {}

        int64_t elapsed_ns() const { return now_ns() - start_ns; }
        int64_t elapsed_us() const { return elapsed_ns() / 1'000; }
        int64_t elapsed_ms() const { return elapsed_ns() / 1'000'000; }

        void reset() { start_ns = now_ns(); }
    };

} // namespace utils
