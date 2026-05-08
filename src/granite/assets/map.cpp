#include "granite/assets/map.hpp"

#include <nlohmann/json.hpp>

namespace gr::Assets {

void Map::load(const std::string& filename) {

}

const std::vector<gr::Scene::Object>& Map::read(void) const {
	return objects_;
}

}
