#pragma once

#include "granite/scene/object.hpp"

#include <string>
#include <vector>

namespace gr::Assets {

class Map {
public:
	void load(const std::string& filename);
	const std::vector<std::unique_ptr<gr::Scene::Object>>& read() const;
private:
	std::vector<std::unique_ptr<gr::Scene::Object>> objects_;
};

}
