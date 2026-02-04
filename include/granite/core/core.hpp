#pragma once

#include "granite/core/color.hpp"
#include "granite/core/math.hpp"
#include "granite/core/vector.hpp"
#include "granite/core/time.hpp"

namespace gr::Core {

struct Config {
    unsigned int depthBits   = 24;
    unsigned int stencilBits = 8;
    unsigned int msaaSamples = 4;
};
    
void init(const Config& cfg);
void init();
void exit();

};