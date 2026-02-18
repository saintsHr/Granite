#include "granite/assets/model.hpp"
#include "granite/core/log.hpp"

#include "tiny_obj_loader/tiny_obj_loader.hpp"

namespace gr::Assets::Model {

gr::Scene::RenderObject load(const std::string& filename) {
    gr::Scene::RenderObject renderObject;
    gr::Scene::RenderObject nullObject;

    // .obj data
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;

    if (!tinyobj::LoadObj(
        &attrib,
        &shapes,
        nullptr,
        nullptr,
        filename.c_str(),
        NULL,
        true
    )){
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::ASSETS,
            "Cannot load model from file '{}'",
            filename
        );
        return nullObject;
    }

    // mesh data
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    // flags
    bool hasNormals = !attrib.normals.empty();
    bool hasUVs = !attrib.texcoords.empty();

    for (auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); i++) {
            tinyobj::index_t idx = shape.mesh.indices[i];

            // vertices
            vertices.push_back(attrib.vertices[3*idx.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3*idx.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3*idx.vertex_index + 2]);

            // normals
            if (hasNormals && idx.normal_index >= 0) {
                normals.push_back(attrib.normals[3*idx.normal_index + 0]);
                normals.push_back(attrib.normals[3*idx.normal_index + 1]);
                normals.push_back(attrib.normals[3*idx.normal_index + 2]);
            } else {
                normals.insert(normals.end(), {0.f, 0.f, 0.f});
            }

            // uvs
            if (hasUVs && idx.texcoord_index >= 0) {
                uvs.push_back(attrib.texcoords[2*idx.texcoord_index + 0]);
                uvs.push_back(attrib.texcoords[2*idx.texcoord_index + 1]);
            } else {
                uvs.insert(uvs.end(), {0.f, 0.f});
            }

            indices.push_back(static_cast<unsigned int>(vertices.size()/3 - 1));
        }
    }

    // loads data into mesh
    renderObject.mesh = new gr::Render::Mesh();
    renderObject.mesh->upload(vertices, indices, normals, uvs);

    return renderObject;
}

}