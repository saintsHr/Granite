#pragma once

#include <string>
#include <vector>

namespace gr::Assets {

class Audio {
public:
	bool load(const std::string& filename);
	std::vector<float> read(void) const;

	long frameCount(void) const;
	int sampleRate(void) const;
	int channelsCount(void) const;
private:
	std::vector<float> buffer_;
	long framesCount_;
	int  sampleRate_;
	int  channelsCount_;
};

};
