#pragma once

#include "granite/scene/object.hpp"

#include <string>
#include <vector>

namespace gr::Assets {

class Map {
public:
	void load(const std::string& filename);
	const std::vector<gr::Scene::Object>& read(void) const;
private:
	std::vector<gr::Scene::Object> objects_;
};

}
