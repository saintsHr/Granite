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

#include <sndfile.h>

#include "granite/assets/audio.hpp"
#include "granite/core/log.hpp"

namespace gr::Assets {

bool Audio::load(const std::string& filename) {
	SF_INFO info = {};
	SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &info);

	if(!file) {
		gr::internal::log(
			gr::internal::Severity::ERROR,
			gr::internal::Module::ASSETS,
			"Cannot open sound file: %s",
			filename.c_str()
		);
		return false;
	}

	buffer_.resize(info.frames * info.channels);
	framesCount_   = info.frames;
	channelsCount_ = info.channels;
	sampleRate_    = info.samplerate;

	long read = sf_readf_float(file, buffer_.data(), info.frames);

	if (read <= 0) {
		gr::internal::log(
			gr::internal::Severity::ERROR,
			gr::internal::Module::ASSETS,
			"Cannot load sound file: %s",
			filename.c_str()
		);
		sf_close(file);
		return false;
	}

	buffer_.resize(read * info.channels);
	framesCount_   = read;

	sf_close(file);
	return true;
}

const std::vector<float>& Audio::read(void) const {
	return buffer_;
}

long Audio::frameCount(void) const {
	return framesCount_;
}

int Audio::sampleRate(void) const {
	return sampleRate_;
}

int Audio::channelsCount(void) const {
	return channelsCount_;
}

}
