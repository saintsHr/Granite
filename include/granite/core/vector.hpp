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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "granite/core/math.hpp"

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

    Vec2 Normalize(float min, float max) {
        return {
            gr::Math::Normalize(this->x, min, max),
            gr::Math::Normalize(this->y, min, max)
        };
    }

    Vec2 Clamp(float min, float max) {
        return {
            gr::Math::Clamp(this->x, min, max),
            gr::Math::Clamp(this->y, min, max)
        };
    }

    float length() const {
        return std::sqrt(x*x + y*y);
    }

    Vec2 invert() {
        return {this->y, this->x};
    }

    Vec2& operator+=(const Vec2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2& operator*=(const Vec2& other) {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vec2& operator/=(const Vec2& other) {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vec2 operator*(float s) const {
        return {
            x * s,
            y * s
        };
    }

    Vec2 operator/(float s) const {
        return {
            x / s,
            y / s
        };
    }

    Vec2 operator+(float s) const {
        return {
            x + s,
            y + s
        };
    }

    Vec2 operator-(float s) const {
        return {
            x - s,
            y - s
        };
    }

    Vec2 operator+(const Vec2& other) const {
        return {
            x + other.x,
            y + other.y
        };
    }

    Vec2 operator-(const Vec2& other) const {
        return {
            x - other.x,
            y - other.y
        };
    }

    Vec2 operator*(const Vec2& other) const {
        return {
            x * other.x,
            y * other.y
        };
    }

    Vec2 operator/(const Vec2& other) const {
        return {
            x / other.x,
            y / other.y
        };
    }

    float x, y;
};

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xv, float yv, float zv) : x(xv), y(yv), z(zv) {}

    Vec3 Normalize(float min, float max) {
        return {
            gr::Math::Normalize(this->x, min, max),
            gr::Math::Normalize(this->y, min, max),
            gr::Math::Normalize(this->z, min, max)
        };
    }

    Vec3 Clamp(float min, float max) {
        return {
            gr::Math::Clamp(this->x, min, max),
            gr::Math::Clamp(this->y, min, max),
            gr::Math::Clamp(this->z, min, max)
        };
    }

    float length() const {
        return std::sqrt(x*x + y*y + z*z);
    }

    Vec3 invert() {
        return {this->z, this->y, this->x};
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vec3& operator*=(const Vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    Vec3& operator/=(const Vec3& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    Vec3 operator*(float s) const {
        return {
            x * s,
            y * s,
            z * s
        };
    }

    Vec3 operator/(float s) const {
        return {
            x / s,
            y / s,
            z / s
        };
    }

    Vec3 operator+(float s) const {
        return {
            x + s,
            y + s,
            z + s
        };
    }

    Vec3 operator-(float s) const {
        return {
            x - s,
            y - s,
            z - s
        };
    }

    Vec3 operator+(const Vec3& other) const {
        return {
            x + other.x,
            y + other.y,
            z + other.z
        };
    }

    Vec3 operator-(const Vec3& other) const {
        return {
            x - other.x,
            y - other.y,
            z - other.z
        };
    }

    Vec3 operator*(const Vec3& other) const {
        return {
            x * other.x,
            y * other.y,
            z * other.z
        };
    }

    Vec3 operator/(const Vec3& other) const {
        return {
            x / other.x,
            y / other.y,
            z / other.z
        };
    }

    float x, y, z;
};

class Transform {
public:
    gr::Vec3 position = {0.0f, 0.0f, 0.0f};
    gr::Vec3 rotation = {0.0f, 0.0f, 0.0f};
    gr::Vec3 scale    = {1.0f, 1.0f, 1.0f};

    glm::mat4 getMatrix() {
        glm::mat4 model(1.0f);

        model = glm::translate(model, glm::vec3(position.x, position.y, position.z));

        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        model = glm::scale(model, glm::vec3(scale.x, scale.y, scale.z));

        return model;
    }
};

}