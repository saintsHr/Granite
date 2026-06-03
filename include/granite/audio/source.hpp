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

#pragma once

#include "granite/assets/sound.hpp"
#include "granite/core/vector.hpp"

namespace gr::Audio {

enum class State {
	PLAYING,
	PAUSED,
	STOPPED
};

class Source {
public:
	Source();
	~Source();

	gr::Vec3 position = {0.0f, 0.0f, 0.0f};

	void load(const gr::Assets::Sound& audio, bool forceMono = true);
	void unload(void);

	void update(void);

	void play(void);
	void pause(void);
	void stop(void);

	void setVolume(float volume);
	void setLooping(bool state);

	float getVolume(void) const;
	State getState(void) const;

	bool isLoaded(void) const;
	bool isLooping(void) const;
private:
	State state_ = State::STOPPED;
	bool loaded_ = false;

	unsigned int buffer_ = 0;
	unsigned int source_ = 0;

	float volume_ = 1.0f;
	bool looping_ = false;
};

}
