#pragma once

namespace gr {

class Vec2 {
public:
    Vec2() : x(0), y(0) {}
    Vec2(float xv, float yv) : x(xv), y(yv) {}

    float x, y;
};

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xv, float yv, float zv) : x(xv), y(yv), z(zv) {}

    float x, y, z;
};

}