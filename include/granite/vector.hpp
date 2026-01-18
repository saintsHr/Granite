#pragma once

#include <glm/glm.hpp>

namespace gr {

class Vec2 {
public:
    Vec2() : x(0), y(0) {}
    Vec2(float xv, float yv) : x(xv), y(yv) {}

    explicit operator glm::vec2() const {
        return {x, y};
    }

    Vec2(const glm::vec2& v) : x(v.x), y(v.y) {}

    Vec2& operator+=(const Vec2& rhs){
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& rhs){
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    Vec2& operator*=(const Vec2& rhs){
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }

    Vec2& operator/=(const Vec2& rhs){
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    Vec2 operator*(float s) const{
        return {x * s, y * s};
    }

    Vec2 operator/(float s) const{
        return {x / s, y / s};
    }

    Vec2 operator+(float s) const{
        return {x + s, y + s};
    }

    Vec2 operator-(float s) const{
        return {x - s, y - s};
    }

    float x, y;
};

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xv, float yv, float zv) : x(xv), y(yv), z(zv) {}

    explicit operator glm::vec3() const {
        return {x, y, z};
    }

    Vec3(const glm::vec3& v) : x(v.x), y(v.y), z(v.z) {}

    Vec3& operator+=(const Vec3& rhs){
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& rhs){
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    Vec3& operator*=(const Vec3& rhs){
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    Vec3& operator/=(const Vec3& rhs){
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    Vec3 operator*(float s) const{
        return {x * s, y * s, z * s};
    }

    Vec3 operator/(float s) const{
        return {x / s, y / s, z / s};
    }

    Vec3 operator+(float s) const{
        return {x + s, y + s, z + s};
    }

    Vec3 operator-(float s) const{
        return {x - s, y - s, z - s};
    }

    float x, y, z;
};

}