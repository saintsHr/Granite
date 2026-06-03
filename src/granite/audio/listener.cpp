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

#include "granite/audio/listener.hpp"
#include "granite/core/math.hpp"

#include <al.h>

static gr::Vec3 calcForward(const gr::Vec2& rotation) {
    float rx = gr::Math::Deg2Rad(rotation.x);
    float ry = gr::Math::Deg2Rad(rotation.y);
    float rz = gr::Math::Deg2Rad(0.0f);

    float cx = std::cos(rx), sx = std::sin(rx);
    float cy = std::cos(ry), sy = std::sin(ry);
    float cz = std::cos(rz), sz = std::sin(rz);

    gr::Vec3 v = {0.0f, 0.0f, -1.0f};

    return {
        v.x*(cy*cz) + v.y*(sx*sy*cz - cx*sz) + v.z*(cx*sy*cz + sx*sz),
        v.x*(cy*sz) + v.y*(sx*sy*sz + cx*cz) + v.z*(cx*sy*sz - sx*cz),
        v.x*(-sy)   + v.y*(sx*cy)            + v.z*(cx*cy)
    };
}

static gr::Vec3 calcUp(const gr::Vec2& rotation) {
    float rx = gr::Math::Deg2Rad(rotation.x);
    float ry = gr::Math::Deg2Rad(rotation.y);
    float rz = gr::Math::Deg2Rad(0.0f);

    float cx = std::cos(rx), sx = std::sin(rx);
    float cy = std::cos(ry), sy = std::sin(ry);
    float cz = std::cos(rz), sz = std::sin(rz);

    gr::Vec3 v = {0.0f, 1.0f, 0.0f};

    return {
        v.x*(cy*cz) + v.y*(sx*sy*cz - cx*sz) + v.z*(cx*sy*cz + sx*sz),
        v.x*(cy*sz) + v.y*(sx*sy*sz + cx*cz) + v.z*(cx*sy*sz - sx*cz),
        v.x*(-sy)   + v.y*(sx*cy)            + v.z*(cx*cy)
    };
}

namespace gr::Audio::Listener {

void setPosition(const gr::Vec3& position) {
	alListener3f(
		AL_POSITION,
		position.x,
		position.y,
		position.z
	);
}

void setRotation(const gr::Vec2& rotation) {
    gr::Vec3 forward = calcForward(rotation);
    gr::Vec3 up      = calcUp(rotation);

    ALfloat orientation[] = {
    	-forward.x,
    	forward.y,
    	forward.z,
    	up.x,
    	up.y,
    	up.z
    };

    alListenerfv(AL_ORIENTATION, orientation);
}

}
