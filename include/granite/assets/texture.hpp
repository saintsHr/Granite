#include "glad/glad.h"
#include "granite/assets/image.hpp"

namespace gr::Assets {

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    void load(const gr::Assets::Image& image);
    GLuint id() {return id_;}
    unsigned char* data() {return data_;}
private:
    unsigned char* data_ = nullptr;
    GLuint id_ = 0;
};

}