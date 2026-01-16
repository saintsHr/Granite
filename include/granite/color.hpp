#pragma once

namespace gr {

class Color3 {
public:
    Color3() : r(0), g(0), b(0) {}
    Color3(int rv, int gv, int bv) : r(rv), g(gv), b(bv) {}

    int r, g, b;
};

}