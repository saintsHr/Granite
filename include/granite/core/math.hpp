#pragma once

#include <cmath>

namespace gr::Math{
    
inline constexpr float PI = 3.1415927f;
inline constexpr float TwoPI = PI * 2.0f;
inline constexpr float HalfPI = PI / 2.0f;

inline float Clamp(float value, float min, float max){
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

inline float Normalize(float value, float min, float max){
    if (std::abs(max - min) < static_cast<float>(1e-9)) return 0.0f;
    if (value <= min) return 0.0f;
    if (value >= max) return 1.0f;
    return (value - min) / (max - min);
}

inline float Smooth(float oldValue, float newValue, float factor){
    return oldValue + (newValue - oldValue) * factor;
}

}