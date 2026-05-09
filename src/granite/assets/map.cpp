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

#include "granite/assets/map.hpp"
#include "granite/core/color.hpp"
#include "granite/core/log.hpp"
#include "granite/renderer/mesh.hpp"
#include "granite/scene/object.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <mutex>

static std::once_flag meshInitFlag;

static std::shared_ptr<gr::Renderer::Mesh> cubeMesh;
static std::shared_ptr<gr::Renderer::Mesh> sphereMesh;
static std::shared_ptr<gr::Renderer::Mesh> circleMesh;
static std::shared_ptr<gr::Renderer::Mesh> coneMesh;
static std::shared_ptr<gr::Renderer::Mesh> cylinderMesh;
static std::shared_ptr<gr::Renderer::Mesh> pyramidMesh;
static std::shared_ptr<gr::Renderer::Mesh> quadMesh;
static std::shared_ptr<gr::Renderer::Mesh> triangleMesh;

static std::shared_ptr<gr::Renderer::Mesh> getMesh(const std::string& name, size_t objIndex, const std::string& filename) {
    if (name == "cube")     return cubeMesh;
    if (name == "sphere")   return sphereMesh;
    if (name == "circle")   return circleMesh;
    if (name == "cone")     return coneMesh;
    if (name == "cylinder") return cylinderMesh;
    if (name == "pyramid")  return pyramidMesh;
    if (name == "quad")     return quadMesh;
    if (name == "triangle") return triangleMesh;

    gr::internal::log(
		gr::internal::Severity::WARNING,
		gr::internal::Module::ASSETS,
		"Object {} loaded from map {} has no valid mesh (defaulting to cube).",
		objIndex, filename
	);

    return cubeMesh;
}

static void ensureMeshes() {
    std::call_once(meshInitFlag, []() {
        cubeMesh     = gr::Renderer::Mesh::newCube();
        sphereMesh   = gr::Renderer::Mesh::newSphere();
        circleMesh   = gr::Renderer::Mesh::newCircle();
        coneMesh     = gr::Renderer::Mesh::newCone();
        cylinderMesh = gr::Renderer::Mesh::newCylinder();
        pyramidMesh  = gr::Renderer::Mesh::newPyramid();
        quadMesh     = gr::Renderer::Mesh::newQuad();
        triangleMesh = gr::Renderer::Mesh::newTriangle();
    });
}

static nlohmann::json openJson(const std::string& filename) {
	std::ifstream file(filename);

	if (!file.is_open()) {
	    gr::internal::log(
			gr::internal::Severity::ERROR,
			gr::internal::Module::ASSETS,
			"Failed to load map from file ('{}').",
			filename
		);
	    return {};
	}

	nlohmann::json json;

	try {
	    file >> json;
	} catch (const std::exception& e) {
		gr::internal::log(
			gr::internal::Severity::ERROR,
			gr::internal::Module::ASSETS,
			"Invalid JSON in {}: {}",
			filename,
			e.what()
		);
	    return {};
	}

	return json;
}

static gr::Vec3 readVec3(const nlohmann::json& j, const char* key) {
    auto it = j.find(key);
	if (it == j.end()) return {0,0,0};

	const auto& v = *it;

    return gr::Vec3(
        v.value("x", 0.0f),
        v.value("y", 0.0f),
        v.value("z", 0.0f)
    );
}

static gr::Color3 readColor3(const nlohmann::json& j, const char* key) {
    auto it = j.find(key);
	if (it == j.end()) return {0,0,0};

	const auto& v = *it;
    
    return gr::Color3(
        v.value("r", 0.0f),
        v.value("g", 0.0f),
        v.value("b", 0.0f)
    );
};

static std::unique_ptr<gr::Scene::Object> parseObject(const nlohmann::json& obj, size_t objIndex, const std::string& filename) {
	auto o = std::make_unique<gr::Scene::Object>();

	o->transform.position = readVec3(obj, "position");
	o->transform.rotation = readVec3(obj, "rotation");
	o->transform.scale    = readVec3(obj, "scale");
	o->material.color     = readColor3(obj, "color");

	std::string meshName = obj.value("mesh","");
	std::shared_ptr<gr::Renderer::Mesh> mesh = getMesh(meshName, objIndex, filename);
		
	o->mesh = mesh;

	return o;
}

namespace gr::Assets {

void Map::load(const std::string& filename) {
	nlohmann::json json = openJson(filename);

	if (json.empty()) {
		gr::internal::log(
    		gr::internal::Severity::ERROR,
			gr::internal::Module::ASSETS,
			"Cannot open map from file ('{}')",
			filename
		);
		return;
	}

	if (!json.contains("objects") || !json["objects"].is_array()) {
    	gr::internal::log(
    		gr::internal::Severity::ERROR,
			gr::internal::Module::ASSETS,
			"Map file has no 'objects' array ('{}')",
			filename
		);
    	return;
	}

	ensureMeshes();

	objects_.clear();
	objects_.reserve(json["objects"].size());

	size_t i = 0;
	for (auto& obj : json["objects"]) {
		auto o = parseObject(obj, i++, filename);
		objects_.push_back(std::move(o));
	}
}

const std::vector<std::unique_ptr<gr::Scene::Object>>& Map::read() const {
	return objects_;
}

}
