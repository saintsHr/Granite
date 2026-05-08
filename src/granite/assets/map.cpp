#include "granite/assets/map.hpp"
#include "granite/renderer/mesh.hpp"
#include "granite/scene/object.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

static gr::Renderer::Mesh* cubeMesh     = nullptr;
static gr::Renderer::Mesh* sphereMesh   = nullptr;
static gr::Renderer::Mesh* circleMesh   = nullptr;
static gr::Renderer::Mesh* coneMesh     = nullptr;
static gr::Renderer::Mesh* cylinderMesh = nullptr;
static gr::Renderer::Mesh* pyramidMesh  = nullptr;
static gr::Renderer::Mesh* quadMesh     = nullptr;
static gr::Renderer::Mesh* triangleMesh = nullptr;

namespace gr::Assets {

void Map::load(const std::string& filename) {
	std::ifstream file(filename);
	nlohmann::json json;

	file >> json;

	for (auto& obj : json["objects"]) {
		if (!cubeMesh)     cubeMesh     = new gr::Renderer::Mesh(gr::Renderer::Mesh::newCube());
		if (!sphereMesh)   sphereMesh   = new gr::Renderer::Mesh(gr::Renderer::Mesh::newSphere());
		if (!circleMesh)   circleMesh   = new gr::Renderer::Mesh(gr::Renderer::Mesh::newCircle());
		if (!coneMesh)     coneMesh     = new gr::Renderer::Mesh(gr::Renderer::Mesh::newCone());
		if (!cylinderMesh) cylinderMesh = new gr::Renderer::Mesh(gr::Renderer::Mesh::newCylinder());
		if (!pyramidMesh)  pyramidMesh  = new gr::Renderer::Mesh(gr::Renderer::Mesh::newPyramid());
		if (!quadMesh)     quadMesh     = new gr::Renderer::Mesh(gr::Renderer::Mesh::newQuad());
		if (!triangleMesh) triangleMesh = new gr::Renderer::Mesh(gr::Renderer::Mesh::newTriangle());

		auto o = std::make_unique<gr::Scene::Object>();

		o->transform.position.x = obj["position"]["x"];
		o->transform.position.y = obj["position"]["y"];
		o->transform.position.z = obj["position"]["z"];

		o->transform.rotation.x = obj["rotation"]["x"];
		o->transform.rotation.y = obj["rotation"]["y"];
		o->transform.rotation.z = obj["rotation"]["z"];

		o->transform.scale.x = obj["scale"]["x"];
		o->transform.scale.y = obj["scale"]["y"];
		o->transform.scale.z = obj["scale"]["z"];

		o->material.color.r = obj["color"]["r"];
		o->material.color.g = obj["color"]["g"];
		o->material.color.b = obj["color"]["b"];

		std::string meshName = obj.value("mesh", "cube");

		if (meshName == "cube") {
		    o->mesh = std::move(*cubeMesh);
		} else if (meshName == "sphere"){
		    o->mesh = std::move(*sphereMesh);
		} else if (meshName == "circle"){
		    o->mesh = std::move(*circleMesh);
		} else if (meshName == "cone"){
		    o->mesh = std::move(*coneMesh);
		} else if (meshName == "cylinder"){
		    o->mesh = std::move(*cylinderMesh);
		} else if (meshName == "pyramid"){
		    o->mesh = std::move(*pyramidMesh);
		} else if (meshName == "quad"){
		    o->mesh = std::move(*quadMesh);
		} else if (meshName == "triangle"){
		    o->mesh = std::move(*triangleMesh);
		}

		objects_.push_back(std::move(o));
	}
}

const std::vector<std::unique_ptr<gr::Scene::Object>>& Map::read() const {
	return objects_;
}

}
