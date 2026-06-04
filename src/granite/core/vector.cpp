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

#include "granite/core/math.hpp"
#include "granite/core/vector.hpp"

namespace gr {

Vec2 Vec2::normalize(float min, float max) {
    return {
        gr::Math::Normalize(this->x, min, max),
        gr::Math::Normalize(this->y, min, max)
    };
}

Vec2 Vec2::clamp(float min, float max) {
    return {
        gr::Math::Clamp(this->x, min, max),
        gr::Math::Clamp(this->y, min, max)
    };
}

float Vec2::length() const {
    return std::sqrt(x*x + y*y);
}

Vec2 Vec2::invert() {
    return {this->y, this->x};
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(const Vec2& other) {
    x *= other.x;
    y *= other.y;
    return *this;
}

Vec2& Vec2::operator/=(const Vec2& other) {
    x /= other.x;
    y /= other.y;
    return *this;
}

Vec2 Vec2::operator*(float s) const {
    return {
        x * s,
        y * s
    };
}

Vec2 Vec2::operator/(float s) const {
    return {
        x / s,
        y / s
    };
}

Vec2 Vec2::operator+(float s) const {
    return {
        x + s,
        y + s
    };
}

Vec2 Vec2::operator-(float s) const {
    return {
        x - s,
        y - s
    };
}

Vec2 Vec2::operator+(const Vec2& other) const {
    return {
        x + other.x,
        y + other.y
    };
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return {
        x - other.x,
        y - other.y
    };
}

Vec2 Vec2::operator*(const Vec2& other) const {
    return {
        x * other.x,
        y * other.y
    };
}

Vec2 Vec2::operator/(const Vec2& other) const {
    return {
        x / other.x,
        y / other.y
    };
}

Vec3 Vec3::normalize(float min, float max) {
    return {
        gr::Math::Normalize(this->x, min, max),
        gr::Math::Normalize(this->y, min, max),
        gr::Math::Normalize(this->z, min, max)
    };
}

Vec3 Vec3::clamp(float min, float max) {
    return {
        gr::Math::Clamp(this->x, min, max),
        gr::Math::Clamp(this->y, min, max),
        gr::Math::Clamp(this->z, min, max)
    };
}

float Vec3::length() const {
    return std::sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::invert() {
    return {this->z, this->y, this->x};
}

Vec3& Vec3::operator+=(const Vec3& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vec3& Vec3::operator*=(const Vec3& other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
}

Vec3& Vec3::operator/=(const Vec3& other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
}

Vec3 Vec3::operator*(float s) const {
    return {
        x * s,
        y * s,
        z * s
    };
}

Vec3 Vec3::operator/(float s) const {
    return {
        x / s,
        y / s,
        z / s
    };
}

Vec3 Vec3::operator+(float s) const {
    return {
        x + s,
        y + s,
        z + s
    };
}

Vec3 Vec3::operator-(float s) const {
    return {
        x - s,
        y - s,
        z - s
    };
}

Vec3 Vec3::operator+(const Vec3& other) const {
    return {
        x + other.x,
        y + other.y,
        z + other.z
    };
}

Vec3 Vec3::operator-(const Vec3& other) const {
    return {
        x - other.x,
        y - other.y,
        z - other.z
    };
}

Vec3 Vec3::operator*(const Vec3& other) const {
    return {
        x * other.x,
        y * other.y,
        z * other.z
    };
}

Vec3 Vec3::operator/(const Vec3& other) const {
    return {
        x / other.x,
        y / other.y,
        z / other.z
    };
}

}
