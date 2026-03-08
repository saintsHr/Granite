/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

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

#include <cmath>

namespace gr::Math{
    
inline constexpr float PI = 3.1415927f;
inline constexpr float TwoPI = PI * 2.0f;
inline constexpr float HalfPI = PI / 2.0f;

inline float Clamp(float value, float min, float max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

inline float Normalize(float value, float min, float max) {
    if (std::abs(max - min) < static_cast<float>(1e-9)) return 0.0f;
    if (value <= min) return 0.0f;
    if (value >= max) return 1.0f;
    return (value - min) / (max - min);
}

inline float Smooth(float oldValue, float newValue, float factor) {
    return oldValue + (newValue - oldValue) * factor;
}

}