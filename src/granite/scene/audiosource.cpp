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

#include "granite/scene/audiosource.hpp"

#include <AL/al.h>
#include <cstdint>
#include <vector>

namespace gr::Scene {

AudioSource::AudioSource() {
	alGenBuffers(1, &buffer_);
	alGenSources(1, &source_);
}

AudioSource::~AudioSource() {
	alDeleteBuffers(1, &buffer_);
	alDeleteSources(1, &source_);
}

void AudioSource::load(const gr::Assets::Audio& audio) {
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

void AudioSource::unload(void) {
	loaded_ = false;
}

bool AudioSource::isLoaded(void) const {
	return loaded_;
}

void AudioSource::play(void) {
	state_ = AudioState::PLAYING;
}

void AudioSource::pause(void) {
	state_ = AudioState::PAUSED;
}

void AudioSource::stop(void) {
	state_ = AudioState::STOPPED;
}

AudioState AudioSource::getState(void) const {
	return state_;
}

}
