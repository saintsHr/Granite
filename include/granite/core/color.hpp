#pragma once

namespace gr {

class Color3 {
public:
    Color3() : r(0), g(0), b(0) {}
    Color3(float rv, float gv, float bv) : r(rv), g(gv), b(bv) {}

    float r, g, b;
};

}