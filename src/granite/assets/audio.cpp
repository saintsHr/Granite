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
