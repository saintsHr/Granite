#include "granite/assets/map.hpp"
#include "granite/core/log.hpp"
#include "granite/renderer/mesh.hpp"
#include "granite/scene/object.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

static std::shared_ptr<gr::Renderer::Mesh> cubeMesh;
static std::shared_ptr<gr::Renderer::Mesh> sphereMesh;
static std::shared_ptr<gr::Renderer::Mesh> circleMesh;
static std::shared_ptr<gr::Renderer::Mesh> coneMesh;
static std::shared_ptr<gr::Renderer::Mesh> cylinderMesh;
static std::shared_ptr<gr::Renderer::Mesh> pyramidMesh;
static std::shared_ptr<gr::Renderer::Mesh> quadMesh;
static std::shared_ptr<gr::Renderer::Mesh> triangleMesh;

namespace gr::Assets {

void Map::load(const std::string& filename) {
	std::ifstream file(filename);
	nlohmann::json json;

	file >> json;

	if (!cubeMesh)     cubeMesh     = gr::Renderer::Mesh::newCube();
	if (!sphereMesh)   sphereMesh   = gr::Renderer::Mesh::newSphere();
	if (!circleMesh)   circleMesh   = gr::Renderer::Mesh::newCircle();
	if (!coneMesh)     coneMesh     = gr::Renderer::Mesh::newCone();
	if (!cylinderMesh) cylinderMesh = gr::Renderer::Mesh::newCylinder();
	if (!pyramidMesh)  pyramidMesh  = gr::Renderer::Mesh::newPyramid();
	if (!quadMesh)     quadMesh     = gr::Renderer::Mesh::newQuad();
	if (!triangleMesh) triangleMesh = gr::Renderer::Mesh::newTriangle();

	int i = 0;
	for (auto& obj : json["objects"]) {
		i++;

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

		std::string meshName = obj.value("mesh","");

		if (meshName == "cube") {
		    o->mesh = cubeMesh;
		} else if (meshName == "sphere"){
		    o->mesh = sphereMesh;
		} else if (meshName == "circle"){
		    o->mesh = circleMesh;
		} else if (meshName == "cone"){
		    o->mesh = coneMesh;
		} else if (meshName == "cylinder"){
		    o->mesh = cylinderMesh;
		} else if (meshName == "pyramid"){
		    o->mesh = pyramidMesh;
		} else if (meshName == "quad"){
		    o->mesh = quadMesh;
		} else if (meshName == "triangle"){
		    o->mesh = triangleMesh;
		} else {
			gr::internal::log(
				gr::internal::Severity::WARNING,
				gr::internal::Module::ASSETS,
				"Object {} loaded from map {} has no valid mesh (defaulting to cube).",
				i, filename
			);

			o->mesh = cubeMesh;
		}

		objects_.push_back(std::move(o));
	}
}

const std::vector<std::unique_ptr<gr::Scene::Object>>& Map::read() const {
	return objects_;
}

}
