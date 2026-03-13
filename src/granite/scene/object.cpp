#include "granite/scene/object.hpp"
#include "granite/renderer/renderer.hpp"

namespace gr::Scene {

void Object::draw() {
    part_.material  = material;
    part_.transform = transform;
    part_.mesh      = &mesh;
    gr::Renderer::addToQueue(part_);
}

gr::Vec3 Object::getDirection(gr::Direction direction) {
    float rx = gr::Math::deg2rad(transform.rotation.x);
    float ry = gr::Math::deg2rad(transform.rotation.y);
    float rz = gr::Math::deg2rad(transform.rotation.z);

    float cx = cos(rx), sx = sin(rx);
    float cy = cos(ry), sy = sin(ry);
    float cz = cos(rz), sz = sin(rz);

    float Rx[3][3] = {{1,0,0},{0,cx,-sx},{0,sx,cx}};
    float Ry[3][3] = {{cy,0,sy},{0,1,0},{-sy,0,cy}};
    float Rz[3][3] = {{cz,-sz,0},{sz,cz,0},{0,0,1}};

    float R[3][3] = {0};
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            for(int k=0;k<3;k++)
                for(int l=0;l<3;l++)
                    R[i][j] += Rz[i][k] * Ry[k][l] * Rx[l][j];

    gr::Vec3 local;
    switch(direction){
        case gr::Direction::FRONT:  local = {0,0,1}; break;
        case gr::Direction::BACK:   local = {0,0,-1}; break;
        case gr::Direction::RIGHT:  local = {1,0,0}; break;
        case gr::Direction::LEFT:   local = {-1,0,0}; break;
        case gr::Direction::UP:     local = {0,1,0}; break;
        case gr::Direction::DOWN:   local = {0,-1,0}; break;
        default: return {0,0,0};
    }

    gr::Vec3 result;
    result.x = R[0][0]*local.x + R[0][1]*local.y + R[0][2]*local.z;
    result.y = R[1][0]*local.x + R[1][1]*local.y + R[1][2]*local.z;
    result.z = R[2][0]*local.x + R[2][1]*local.y + R[2][2]*local.z;

    return result;
}

void ModelObject::load(gr::Assets::Model& model, std::shared_ptr<gr::Renderer::Shader> shader) {
    parts_.clear();
    parts_.reserve(model.meshes.size());

    for (size_t i = 0; i < model.meshes.size(); i++) {
        parts_.emplace_back();
        auto& obj = parts_.back();

        obj.mesh = &model.meshes[i];
        obj.material = model.materials[i];
        if (shader) obj.material.shader = shader;
        obj.transform = transform;
    }
}

void ModelObject::draw() {
    for (auto& p : parts_) {
        p.transform = transform;
        gr::Renderer::addToQueue(p);
    }
}

gr::Vec3 ModelObject::getDirection(gr::Direction direction) {
    float rx = gr::Math::deg2rad(transform.rotation.x);
    float ry = gr::Math::deg2rad(transform.rotation.y);
    float rz = gr::Math::deg2rad(transform.rotation.z);

    float cx = cos(rx), sx = sin(rx);
    float cy = cos(ry), sy = sin(ry);
    float cz = cos(rz), sz = sin(rz);

    float Rx[3][3] = {{1,0,0},{0,cx,-sx},{0,sx,cx}};
    float Ry[3][3] = {{cy,0,sy},{0,1,0},{-sy,0,cy}};
    float Rz[3][3] = {{cz,-sz,0},{sz,cz,0},{0,0,1}};

    float R[3][3] = {0};
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            for(int k=0;k<3;k++)
                for(int l=0;l<3;l++)
                    R[i][j] += Rz[i][k] * Ry[k][l] * Rx[l][j];

    gr::Vec3 local;
    switch(direction){
        case gr::Direction::FRONT:  local = {0,0,1}; break;
        case gr::Direction::BACK:   local = {0,0,-1}; break;
        case gr::Direction::RIGHT:  local = {1,0,0}; break;
        case gr::Direction::LEFT:   local = {-1,0,0}; break;
        case gr::Direction::UP:     local = {0,1,0}; break;
        case gr::Direction::DOWN:   local = {0,-1,0}; break;
        default: return {0,0,0};
    }

    gr::Vec3 result;
    result.x = R[0][0]*local.x + R[0][1]*local.y + R[0][2]*local.z;
    result.y = R[1][0]*local.x + R[1][1]*local.y + R[1][2]*local.z;
    result.z = R[2][0]*local.x + R[2][1]*local.y + R[2][2]*local.z;

    return result;
}

}