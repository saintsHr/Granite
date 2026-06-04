/*
MIT License

Copyright (c) 2026 saintsHr

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <chrono>

namespace gr::Time {

using Clock = std::chrono::steady_clock;

inline float getElapsedTimeMS() {
    static auto start = Clock::now();
    auto now = Clock::now();
    return std::chrono::duration<float, std::milli>(now - start).count();
}

inline float getElapsedTimeS() {
    return getElapsedTimeMS() * 0.001f;
}

inline float getDeltaTimeMS() {
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

inline float getDeltaTimeS() {
    return getDeltaTimeMS() * 0.001f;
}

inline float getFPS(float dt) {
    static float lastFPS = 0.0f;
    if (dt <= 0.0f) return lastFPS;
    lastFPS = 1.0f / dt;
    return lastFPS;
}

}
