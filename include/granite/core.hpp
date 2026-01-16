#pragma once

namespace gr{

class Core{
public:

    Core() = delete;
    
    static void init();
    static void exit();

};

}