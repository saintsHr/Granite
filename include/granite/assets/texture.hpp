#include "glad/glad.h"
#include "granite/assets/image.hpp"

namespace gr::Assets {

enum class TextureFilter {
    NEAREST,
    LINEAR
};

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    void load(const gr::Assets::Image& image);
    void setFilter(TextureFilter filter, bool useMipmaps);

    GLuint id() {return id_;}
    unsigned char* data() {return data_;}
private:
    unsigned char* data_ = nullptr;
    GLuint id_ = 0;
    TextureFilter filter_ = TextureFilter::LINEAR;
    bool useMipmaps_ = true;
};

}