#include "granite/assets/model.hpp"
#include "granite/core/log.hpp"

#include "tiny_obj_loader/tiny_obj_loader.hpp"

namespace gr::Assets::Model {

gr::Render::Mesh* load(const std::string& filename) {
    gr::Render::Mesh* mesh = new gr::Render::Mesh();

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
        return nullptr;
    }

    // mesh data
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    // flags
    bool hasNormals = !attrib.normals.empty();
    bool hasUVs = !attrib.texcoords.empty();

    size_t indexCount = 0;
    unsigned int currentIndex = 0;

    for (auto& shape : shapes) indexCount += shape.mesh.indices.size();
    vertices.reserve(indexCount * 3);
    normals.reserve(indexCount * 3);
    uvs.reserve(indexCount * 2);
    indices.reserve(indexCount);

    for (auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); i++) {
            tinyobj::index_t idx = shape.mesh.indices[i];

            // vertices
            const float* v = &attrib.vertices[static_cast<unsigned long int>(3 * idx.vertex_index)];
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);

            // normals
            if (hasNormals && idx.normal_index >= 0) {
                const float* n = &attrib.normals[static_cast<unsigned long int>(3 * idx.normal_index)];
                normals.push_back(n[0]);
                normals.push_back(n[1]);
                normals.push_back(n[2]);
            } else {
                normals.push_back(0.f);
                normals.push_back(0.f);
                normals.push_back(0.f);
            }

            // uvs
            if (hasUVs && idx.texcoord_index >= 0) {
                const float* t = &attrib.texcoords[static_cast<unsigned long int>(2 * idx.texcoord_index)];
                uvs.push_back(t[0]);
                uvs.push_back(t[1]);
            } else {
                uvs.push_back(0.f);
                uvs.push_back(0.f);
            }

            indices.push_back(currentIndex++);
        }
    }

    // loads data into mesh
    mesh->upload(vertices, indices, normals, uvs);

    return mesh;
}

}