#pragma once

#include <chrono>

namespace gr::Time{

using Clock = std::chrono::steady_clock;

inline float GetElapsedTimeMS(){
    static auto start = Clock::now();
    auto now = Clock::now();
    return std::chrono::duration<float, std::milli>(now - start).count();
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

inline float GetElapsedTimeS(){
    return GetElapsedTimeMS() * 0.001f;
}

inline float GetFPS(){
    static int frames = 0;
    static float elapsedMS = 0.0f;
    static float fps = 0.0f;

    frames++;
    elapsedMS += GetDeltaTimeMS();

    if (elapsedMS >= 1000.0f){
        fps = frames * (1000.0f / elapsedMS);
        frames = 0;
        elapsedMS = 0.0f;
    }

    return fps;
}

}