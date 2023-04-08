#pragma once

#include <chrono>

class TimeNow{
public:
    static long long get_mil_time_now() {
        std::chrono::steady_clock::duration d = std::chrono::steady_clock::now().time_since_epoch();
        return (std::chrono::duration_cast<std::chrono::milliseconds>(d)).count();
    }
};

