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

#include "granite/assets/model.hpp"
#include "granite/core/log.hpp"

#include "tiny_obj_loader/tiny_obj_loader.hpp"

namespace gr::Assets::Model {

gr::Render::Mesh load(const std::string& filename) {
    gr::Render::Mesh mesh;
    gr::Render::Mesh nullmesh;

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
        return nullmesh;
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
    mesh.upload(vertices, indices, normals, uvs);

    return mesh;
}

}