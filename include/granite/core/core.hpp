#pragma once

#include "granite/core/color.hpp"
#include "granite/core/math.hpp"
#include "granite/core/vector.hpp"
#include "granite/core/time.hpp"

namespace gr{

class Core{
public:

    Core() = delete;
    
    static void init();
    static void exit();

};

}