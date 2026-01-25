#pragma once

#include <chrono>

namespace gr::Time{

using Clock = std::chrono::steady_clock;

inline float GetElapsedTimeMS(){
    static auto start = Clock::now();
    auto now = Clock::now();
    return std::chrono::duration<float, std::milli>(now - start).count();
}

inline float GetElapsedTimeS(){
    return GetElapsedTimeMS() * 0.001f;
}

inline float GetDeltaTimeMS(){
    static bool first = true;
    static auto last = Clock::now();

    auto now = Clock::now();

    if (first){
        first = false;
        last = now;
        return 0.0f;
    }

    float dt = std::chrono::duration<float, std::milli>(now - last).count();
    last = now;
    return dt;
}

inline float GetDeltaTimeS(){
    return GetDeltaTimeMS() * 0.001f;
}

inline float GetFPS(float dt){
    static float lastFPS = 0.0f;
    if (dt <= 0.0f) return lastFPS;
    lastFPS = 1.0f / dt;
    return lastFPS;
}

}