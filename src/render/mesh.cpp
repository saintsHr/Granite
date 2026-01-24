#include "granite/render/mesh.hpp"
#include "granite/core/math.hpp"

#include <cmath>

static inline void pushColor(std::vector<float>& colors, const gr::Color3& c){
    colors.push_back(c.r / 255.0f);
    colors.push_back(c.g / 255.0f);
    colors.push_back(c.b / 255.0f);
}

namespace gr::Render{

void Mesh::upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index, const std::vector<float>& color){
    vertexCount_ = static_cast<uint32_t>(vertices.size() / 3);
    indexCount_ = static_cast<uint32_t>(index.size());

    GLsizeiptr vertSize  = static_cast<GLsizeiptr>(vertices.size() * sizeof(float));
    GLsizeiptr colorSize = static_cast<GLsizeiptr>(color.size() * sizeof(float));

    // bind buffers
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    // fills VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertSize + colorSize,
        nullptr,
        GL_STATIC_DRAW
    );

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        vertSize,
        vertices.data()
    );

    glBufferSubData(
        GL_ARRAY_BUFFER,
        vertSize,
        colorSize,
        color.data()
    );

    // fills EBO data
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(index.size() * sizeof(unsigned int)),
        index.data(),
        GL_STATIC_DRAW
    );

    // configs VAO
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        static_cast<void*>(0)
    );
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(vertSize)
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void Mesh::draw(Shader shader, GLenum drawMode) const{
    shader.use();

    glBindVertexArray(vao_);

    // draws the mesh
    if(indexCount_ > 0){
        glDrawElements(drawMode, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, static_cast<void*>(0));
    }else{
        glDrawArrays(drawMode, 0, static_cast<GLsizei>(vertexCount_));
    }

    glBindVertexArray(0);
}

void Mesh::newTriangle(const gr::Color3 color){
    std::vector<float> vertices = {
        -1.f, -0.866f, 0.f,
         1.f, -0.866f, 0.f,
         0.f,  0.866f, 0.f
    };

    std::vector<unsigned int> index = { 0, 1, 2 };

    std::vector<float> colors;
    for (int i = 0; i < 3; i++)
        pushColor(colors, color);

    upload(vertices, index, colors);
}

void Mesh::newQuad(const gr::Color3 color){
    std::vector<float> vertices = {
        -1.f, -1.f, 0.f,
         1.f, -1.f, 0.f,
         1.f,  1.f, 0.f,
        -1.f,  1.f, 0.f
    };

    std::vector<unsigned int> index = {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<float> colors;
    for (int i = 0; i < 4; i++)
        pushColor(colors, color);

    upload(vertices, index, colors);
}

void Mesh::newCube(const gr::Color3 color){
    std::vector<float> vertices = {
        -1,-1,-1,  1,-1,-1,  1, 1,-1, -1, 1,-1,
        -1,-1, 1,  1,-1, 1,  1, 1, 1, -1, 1, 1
    };

    std::vector<unsigned int> index = {
        0,1,2, 2,3,0,
        4,5,6, 6,7,4,
        0,4,7, 7,3,0,
        1,5,6, 6,2,1,
        3,2,6, 6,7,3,
        0,1,5, 5,4,0
    };

    std::vector<float> colors;
    for (int i = 0; i < 8; i++)
        pushColor(colors, color);

    upload(vertices, index, colors);
}

void Mesh::newCircle(const gr::Color3 color, int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;
    std::vector<float> colors;

    vertices.insert(vertices.end(), {0.f, 0.f, 0.f});
    pushColor(colors, color);

    for (int i = 0; i < segments; i++){
        float angle = 2.0f * gr::Math::PI * (float(i) / segments);
        vertices.push_back(cos(angle));
        vertices.push_back(sin(angle));
        vertices.push_back(0.f);
        pushColor(colors, color);
    }

    for (int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;
        index.insert(index.end(), {0, (unsigned)i, (unsigned)next});
    }

    upload(vertices, index, colors);
}

void Mesh::newSphere(const gr::Color3 color, int latSeg, int lonSeg){
    std::vector<float> vertices;
    std::vector<unsigned int> index;
    std::vector<float> colors;

    for (int lat = 0; lat <= latSeg; lat++){
        float theta = lat * gr::Math::PI / latSeg;
        for (int lon = 0; lon <= lonSeg; lon++){
            float phi = lon * 2.f * gr::Math::PI / lonSeg;

            float x = sin(theta) * cos(phi);
            float y = cos(theta);
            float z = sin(theta) * sin(phi);

            vertices.insert(vertices.end(), {x,y,z});
            pushColor(colors, color);
        }
    }

    for (int lat = 0; lat < latSeg; lat++){
        for (int lon = 0; lon < lonSeg; lon++){
            int a = lat * (lonSeg + 1) + lon;
            int b = a + lonSeg + 1;

            index.insert(index.end(), {
                (unsigned)a, (unsigned)b, (unsigned)(a+1),
                (unsigned)b, (unsigned)(b+1), (unsigned)(a+1)
            });
        }
    }

    upload(vertices, index, colors);
}

void Mesh::newCylinder(const gr::Color3 color, int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;
    std::vector<float> colors;

    vertices.insert(vertices.end(), {0.f, -1.f, 0.f});
    pushColor(colors, color);

    for(int i = 0; i < segments; i++){
        float a = 2.f * gr::Math::PI * (float(i) / segments);
        float x = cos(a);
        float z = sin(a);

        vertices.insert(vertices.end(), {x, -1.f, z});
        pushColor(colors, color);
    }

    unsigned int topCenter = vertices.size() / 3;
    vertices.insert(vertices.end(), {0.f, 1.f, 0.f});
    pushColor(colors, color);

    unsigned int topRingStart = vertices.size() / 3;
    for(int i = 0; i < segments; i++){
        float a = 2.f * gr::Math::PI * (float(i) / segments);
        float x = cos(a);
        float z = sin(a);

        vertices.insert(vertices.end(), {x, 1.f, z});
        pushColor(colors, color);
    }

    for(int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;
        index.insert(index.end(), {0u, (unsigned)next, (unsigned)i});
    }

    for(int i = 0; i < segments; i++){
        int curr = topRingStart + i;
        int next = topRingStart + (i + 1) % segments;
        index.insert(index.end(), {
            (unsigned)topCenter,
            (unsigned)curr,
            (unsigned)next
        });
    }

    for(int i = 0; i < segments; i++){
        int b0 = 1 + i;
        int b1 = 1 + (i + 1) % segments;
        int t0 = topRingStart + i;
        int t1 = topRingStart + (i + 1) % segments;

        index.insert(index.end(), {
            (unsigned)b0, (unsigned)t0, (unsigned)b1,
            (unsigned)b1, (unsigned)t0, (unsigned)t1
        });
    }

    upload(vertices, index, colors);
}

void Mesh::newPyramid(const gr::Color3 color){
    std::vector<float> vertices = {
         1.f, -1.f,  1.f,
        -1.f, -1.f,  1.f,
        -1.f, -1.f, -1.f,
         1.f, -1.f, -1.f,
         0.f,  1.f,  0.f
    };

    std::vector<unsigned int> index = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
    };

    std::vector<float> colors;
    for(int i = 0; i < 5; i++)
        pushColor(colors, color);

    upload(vertices, index, colors);
}

void Mesh::newCone(const gr::Color3 color, int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;
    std::vector<float> colors;

    vertices.insert(vertices.end(), {0.f, 1.f, 0.f});
    pushColor(colors, color);

    unsigned int baseCenter = vertices.size() / 3;
    vertices.insert(vertices.end(), {0.f, -1.f, 0.f});
    pushColor(colors, color);

    unsigned int ringStart = vertices.size() / 3;
    for(int i = 0; i < segments; i++){
        float a = 2.f * gr::Math::PI * (float(i) / segments);
        float x = cos(a);
        float z = sin(a);

        vertices.insert(vertices.end(), {x, -1.f, z});
        pushColor(colors, color);
    }

    for(int i = 0; i < segments; i++){
        int curr = ringStart + i;
        int next = ringStart + (i + 1) % segments;
        index.insert(index.end(), {
            0u,
            (unsigned)curr,
            (unsigned)next
        });
    }

    for(int i = 0; i < segments; i++){
        int curr = ringStart + i;
        int next = ringStart + (i + 1) % segments;
        index.insert(index.end(), {
            (unsigned)baseCenter,
            (unsigned)next,
            (unsigned)curr
        });
    }

    upload(vertices, index, colors);
}

Mesh::~Mesh(){
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
}

Mesh::Mesh(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}
    
}