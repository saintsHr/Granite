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
#include <filesystem>
#include <unordered_map>

namespace std {
    template<> struct hash<glm::vec3> {
        size_t operator()(const glm::vec3& v) const {
            return ((std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1)) >> 1) ^ (std::hash<float>()(v.z) << 1);
        }
    };
    template<> struct hash<glm::vec2> {
        size_t operator()(const glm::vec2& v) const {
            return (std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1));
        }
    };
}

struct Vertex {
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 uv;
    bool operator==(const Vertex& other) const {
        return pos == other.pos && normal == other.normal && uv == other.uv;
    }
};

namespace gr::Assets {

static void computeNormal(const float v0[3], const float v1[3], const float v2[3], float out[3]) {
    float e1x = v1[0] - v0[0];
    float e1y = v1[1] - v0[1];
    float e1z = v1[2] - v0[2];

    float e2x = v2[0] - v0[0];
    float e2y = v2[1] - v0[1];
    float e2z = v2[2] - v0[2];

    out[0] = e1y * e2z - e1z * e2y;
    out[1] = e1z * e2x - e1x * e2z;
    out[2] = e1x * e2y - e1y * e2x;

    float len = std::sqrt(out[0]*out[0] + out[1]*out[1] + out[2]*out[2]);
    if (len > 0.0f) {
        out[0] /= len;
        out[1] /= len;
        out[2] /= len;
    }
}

gr::Assets::Model Model::upload(const std::string& filename) {
    gr::Assets::Model model;
    gr::Assets::Model nullmodel;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> objMaterials;

    std::string basedir;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos)
        basedir = filename.substr(0, pos + 1);
    else
        basedir = "./";

    if (!tinyobj::LoadObj(&attrib, &shapes, &objMaterials, nullptr, filename.c_str(), basedir.c_str(), true)) {
        gr::internal::log(
            gr::internal::Severity::ERROR,
            gr::internal::Module::ASSETS,
            "Cannot load model from file '{}'",
            filename
        );
        return nullmodel;
    }

    bool hasNormals = !attrib.normals.empty();
    bool hasUVs = !attrib.texcoords.empty();

    for (auto& shape : shapes) {
        size_t indexOffset = 0;
        const auto& numFaceVerts = shape.mesh.num_face_vertices;

        std::unordered_map<int, std::vector<tinyobj::index_t>> facesByMaterial;

        for (size_t f = 0; f < numFaceVerts.size(); ++f) {
            size_t fv = numFaceVerts[f];
            int matID = -1;
            if (f < shape.mesh.material_ids.size())
                matID = shape.mesh.material_ids[f];

            for (size_t v = 0; v < fv; ++v) {
                facesByMaterial[matID].push_back(shape.mesh.indices[indexOffset + v]);
            }

            indexOffset += fv;
        }

        for (auto& [matID, indicesList] : facesByMaterial) {
            std::vector<float> vertices;
            std::vector<float> normals;
            std::vector<float> uvs;
            std::vector<unsigned int> indices;
            unsigned int currentIndex = 0;

            for (size_t i = 0; i < indicesList.size(); i += 3) {
                float faceNormal[3] = {0,0,0};

                if (!hasNormals) {
                    const float* v0 = &attrib.vertices[3 * static_cast<size_t>(indicesList[i+0].vertex_index)];
                    const float* v1 = &attrib.vertices[3 * static_cast<size_t>(indicesList[i+1].vertex_index)];
                    const float* v2 = &attrib.vertices[3 * static_cast<size_t>(indicesList[i+2].vertex_index)];

                    computeNormal(v0, v1, v2, faceNormal);
                }

                for (int k = 0; k < 3; k++)
                {
                    auto& idx = indicesList[static_cast<size_t>(i) + static_cast<size_t>(k)];

                    const float* v = &attrib.vertices[3 * static_cast<size_t>(idx.vertex_index)];
                    vertices.push_back(v[0]);
                    vertices.push_back(v[1]);
                    vertices.push_back(v[2]);

                    // normal
                    if (hasNormals && idx.normal_index >= 0) {
                        const float* n = &attrib.normals[3 * static_cast<size_t>(idx.normal_index)];
                        normals.push_back(n[0]);
                        normals.push_back(n[1]);
                        normals.push_back(n[2]);
                    } else {
                        normals.push_back(faceNormal[0]);
                        normals.push_back(faceNormal[1]);
                        normals.push_back(faceNormal[2]);
                    }

                    // uv
                    if (hasUVs && idx.texcoord_index >= 0) {
                        const float* t = &attrib.texcoords[2 * static_cast<size_t>(idx.texcoord_index)];
                        uvs.push_back(t[0]);
                        uvs.push_back(t[1]);
                    } else {
                        uvs.push_back(0.f);
                        uvs.push_back(0.f);
                    }

                    indices.push_back(currentIndex++);
                }
            }

            gr::Renderer::Mesh mesh;
            mesh.upload(vertices, indices, normals, uvs);
            model.meshes.push_back(std::move(mesh));

            gr::Renderer::Material material;
            if (matID >= 0 && static_cast<size_t>(matID) < objMaterials.size()) {
                const auto& m = objMaterials[static_cast<size_t>(matID)];
                material.color = { m.diffuse[0]*255.f, m.diffuse[1]*255.f, m.diffuse[2]*255.f };
                material.specularColor = { m.specular[0]*255.f, m.specular[1]*255.f, m.specular[2]*255.f };
                material.shininess = m.shininess;
                material.opacity   = m.dissolve;

                if (!m.diffuse_texname.empty()) {
                    gr::Assets::Image img;
                    img.load(basedir + m.diffuse_texname);
                    material.texture.load(img);
                }
            }
            model.materials.push_back(material);
        }
    }

    return model;
}

}