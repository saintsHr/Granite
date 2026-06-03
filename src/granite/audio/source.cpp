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

#include "granite/audio/source.hpp"
#include "granite/core/math.hpp"

#include <AL/al.h>
#include <cstdint>
#include <vector>

namespace gr::Audio {

Source::Source() {
	alGenBuffers(1, &buffer_);
	alGenSources(1, &source_);
}

Source::~Source() {
	unload();
	alDeleteSources(1, &source_);
	alDeleteBuffers(1, &buffer_);
}

void Source::load(const gr::Assets::Sound& audio) {
	if (audio.isLoaded()) {
		std::vector<int16_t> samples;
		int format;

		samples.reserve(audio.frameCount() * audio.channelsCount());

		switch (audio.channelsCount()) {
			case 1: format = AL_FORMAT_MONO16;   break;
			case 2: format = AL_FORMAT_STEREO16; break;
			default: break;
		}

		for (uint64_t i = 0; i < audio.frameCount() * audio.channelsCount(); i++) {
			samples.push_back(audio.read().at(i) * 32767);
		}

		alBufferData(
			buffer_,
			format,
			samples.data(),
			samples.size() * sizeof(int16_t),
			audio.sampleRate()
		);

		alSourcei(source_, AL_BUFFER, buffer_);

		loaded_ = true;
	}
}

void Source::unload(void) {
	if (state_ != State::STOPPED) stop();
	alSourcei(source_, AL_BUFFER, 0);
	loaded_ = false;
}

void Source::update(void) {
	alSource3f(
		source_,
		AL_POSITION,
		position.x,
		position.y,
		position.z
	);
}

void Source::setVolume(float volume) {
	volume = gr::Math::Clamp(volume, 0.0f, 1.0f);

	alSourcef(source_, AL_GAIN, volume);

	volume_ = volume;
}

void Source::setLooping(bool state) {
	alSourcei(source_, AL_LOOPING, state ? AL_TRUE : AL_FALSE);
	looping_ = state;
}

float Source::getVolume(void) const {
	return volume_;
}

bool Source::isLoaded(void) const {
	return loaded_;
}

bool Source::isLooping(void) const {
	return looping_;
}

void Source::play(void) {
	if (loaded_ && state_ != State::PLAYING) {
		alSourcePlay(source_);
		state_ = State::PLAYING;
	}
}

void Source::pause(void) {
	if (loaded_ && state_ != State::PAUSED) {
		alSourcePause(source_);
		state_ = State::PAUSED;
	}
}

void Source::stop(void) {
	if (loaded_ && state_ != State::STOPPED) {
		alSourceStop(source_);
		state_ = State::STOPPED;
	}
}

State Source::getState(void) const {
	return state_;
}

}
