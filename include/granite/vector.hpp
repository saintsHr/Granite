#pragma once

namespace gr {

class Vec2 {
public:
    Vec2() : x(0), y(0) {}
    Vec2(int xv, int yv) : x(xv), y(yv) {}

    int x, y;
};

class Vec3 {
public:
    Vec3() : x(0), y(0), z(0) {}
    Vec3(int xv, int yv, int zv) : x(xv), y(yv), z(zv) {}

    int x, y, z;
};

}