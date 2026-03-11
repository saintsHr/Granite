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

namespace gr::Assets {

gr::Assets::Model Model::upload(const std::string& filename) {
    gr::Assets::Model model;
    gr::Assets::Model nullmodel;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> objMaterials;

    if (!tinyobj::LoadObj(
        &attrib,
        &shapes,
        &objMaterials,
        nullptr,
        filename.c_str(),
        NULL,
        true
    )) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::ASSETS,
            "Cannot load model from file '{}'",
            filename
        );
        return nullmodel;
    }

    model.meshes_.clear();
    model.materials_.clear();

    bool hasNormals = !attrib.normals.empty();
    bool hasUVs = !attrib.texcoords.empty();

    for (auto& shape : shapes) {

        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> uvs;
        std::vector<unsigned int> indices;

        unsigned int currentIndex = 0;

        for (size_t i = 0; i < shape.mesh.indices.size(); i++) {

            tinyobj::index_t idx = shape.mesh.indices[i];

            // vertex
            const float* v = &attrib.vertices[3 * idx.vertex_index];
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);

            // normal
            if (hasNormals && idx.normal_index >= 0) {
                const float* n = &attrib.normals[3 * idx.normal_index];
                normals.push_back(n[0]);
                normals.push_back(n[1]);
                normals.push_back(n[2]);
            } else {
                normals.push_back(0.f);
                normals.push_back(0.f);
                normals.push_back(0.f);
            }

            // uv
            if (hasUVs && idx.texcoord_index >= 0) {
                const float* t = &attrib.texcoords[2 * idx.texcoord_index];
                uvs.push_back(t[0]);
                uvs.push_back(t[1]);
            } else {
                uvs.push_back(0.f);
                uvs.push_back(0.f);
            }

            indices.push_back(currentIndex++);
        }

        // create mesh
        gr::Render::Mesh mesh;
        mesh.upload(vertices, indices, normals, uvs);
        model.meshes_.push_back(std::move(mesh));

        // create material
        gr::Render::Material material;

        int matID = -1;
        if (!shape.mesh.material_ids.empty())
            matID = shape.mesh.material_ids[0];

        if (matID >= 0 && static_cast<size_t>(matID) < objMaterials.size()) {

            const auto& m = objMaterials[matID];

            material.color = {
                m.diffuse[0] * 255.0f,
                m.diffuse[1] * 255.0f,
                m.diffuse[2] * 255.0f
            };

            material.specularColor = {
                m.specular[0] * 255.0f,
                m.specular[1] * 255.0f,
                m.specular[2] * 255.0f
            };

            material.shininess = m.shininess;
            material.opacity   = m.dissolve;

            if (!m.diffuse_texname.empty()) {
                gr::Assets::Image img;
                img.load(m.diffuse_texname);
                material.texture.load(img);
            }
        }

        model.materials_.push_back(material);
    }

    return model;
}

}