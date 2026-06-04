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

namespace gr {

enum class Direction {
    FRONT,
    BACK,
    RIGHT,
    LEFT,
    DOWN,
    UP
};

class Vec2 {
public:
    Vec2() : x(0), y(0) {}
    Vec2(float xv, float yv) : x(xv), y(yv) {}

    Vec2 normalize(float min, float max);
    Vec2 clamp(float min, float max);
    Vec2 invert();
    float length() const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(const Vec2& other);
    Vec2& operator/=(const Vec2& other);
    Vec2 operator*(float s) const;
    Vec2 operator/(float s) const;
    Vec2 operator+(float s) const;
    Vec2 operator-(float s) const;
    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(const Vec2& other) const;
    Vec2 operator/(const Vec2& other) const;

    float x, y;
};

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xv, float yv, float zv) : x(xv), y(yv), z(zv) {}

    Vec3 normalize(float min, float max);
    Vec3 clamp(float min, float max);
    Vec3 invert();
    float length() const;

    Vec3& operator+=(const Vec3& other);
    Vec3& operator-=(const Vec3& other);
    Vec3& operator*=(const Vec3& other);
    Vec3& operator/=(const Vec3& other);
    Vec3 operator*(float s) const;
    Vec3 operator/(float s) const;
    Vec3 operator+(float s) const;
    Vec3 operator-(float s) const;
    Vec3 operator+(const Vec3& other) const;
    Vec3 operator-(const Vec3& other) const;
    Vec3 operator*(const Vec3& other) const;
    Vec3 operator/(const Vec3& other) const;

    float x, y, z;
};

class Transform {
public:
    gr::Vec3 position = {0.0f, 0.0f, 0.0f};
    gr::Vec3 rotation = {0.0f, 0.0f, 0.0f};
    gr::Vec3 scale    = {1.0f, 1.0f, 1.0f};
};

}
