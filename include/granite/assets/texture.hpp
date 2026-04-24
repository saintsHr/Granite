/*
MIT License

Copyright (c) 2026 Henrique Rodrigues Santos

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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

    unsigned int id() {return id_;}
    unsigned char* data() {return data_;}
private:
    unsigned char* data_ = nullptr;
    unsigned int id_ = 0;
    TextureFilter filter_ = TextureFilter::LINEAR;
    bool useMipmaps_ = true;
};

}