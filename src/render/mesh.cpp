#include "granite/render/mesh.hpp"
#include "granite/core/math.hpp"

#include <cmath>

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
        -1.0f, -0.866f, 0.0f,
         1.0f, -0.866f, 0.0f,
         0.0f,  0.866f, 0.0f,
    };
    std::vector<unsigned int> index = {
        0, 1, 2
    };
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}
void Mesh::newQuad(const gr::Color3 color){
    std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,
    };
    std::vector<unsigned int> index = {
        0, 1, 2,
        0, 3, 2
    };
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}

void Mesh::newCube(const gr::Color3 color){
    std::vector<float> vertices = {
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,
         1.0f,  1.0f, 1.0f,
        -1.0f,  1.0f, 1.0f,
    };
    std::vector<unsigned int> index = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        6, 5, 1,
        1, 2, 6,
        0, 4, 7,
        0, 7, 3,
        4, 1, 3,
        4, 5, 1,
        7, 6, 2,
        7, 2, 3,
    };
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}

void Mesh::newCircle(const gr::Color3 color, int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for(int i = 0; i < segments; i++){
        float angle = 2.0f * gr::Math::PI * static_cast<float>(i) / static_cast<float>(segments);
        vertices.push_back(static_cast<float>(cos(angle)));
        vertices.push_back(static_cast<float>(sin(angle)));
        vertices.push_back(0.0f);
    }

    for(int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;
        index.push_back(0);
        index.push_back(static_cast<unsigned int>(i));
        index.push_back(static_cast<unsigned int>(next));
    }
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}

void Mesh::newSphere(const gr::Color3 color, int latSegments, int longSegments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    for(int lat = 0; lat <= latSegments; lat++){
        float theta = static_cast<float>(lat) * gr::Math::PI / static_cast<float>(latSegments);
        float sinTheta = static_cast<float>(sin(theta));
        float cosTheta = static_cast<float>(cos(theta));

        for(int lon = 0; lon <= longSegments; lon++){
            float phi = static_cast<float>(lon) * 2.0f * gr::Math::PI / static_cast<float>(longSegments);
            float sinPhi = static_cast<float>(sin(phi));
            float cosPhi = static_cast<float>(cos(phi));

            float x = sinTheta * cosPhi;
            float y = cosTheta;
            float z = sinTheta * sinPhi;

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    for(int lat = 0; lat < latSegments; lat++){
        for(int lon = 0; lon < longSegments; lon++){
            int first = lat * (longSegments + 1) + lon;
            int second = first + longSegments + 1;

            index.push_back(static_cast<unsigned int>(first));
            index.push_back(static_cast<unsigned int>(second));
            index.push_back(static_cast<unsigned int>(first + 1));

            index.push_back(static_cast<unsigned int>(second));
            index.push_back(static_cast<unsigned int>(second + 1));
            index.push_back(static_cast<unsigned int>(first + 1));
        }
    }
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}

void Mesh::newCylinder(const gr::Color3 color, int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.14159265f * static_cast<float>(i / segments);
        float x = static_cast<float>(cos(angle));
        float z = static_cast<float>(sin(angle));
        vertices.push_back(x);
        vertices.push_back(-1.0f);
        vertices.push_back(z);
    }

    vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.14159265f * static_cast<float>(i / segments);
        float x = static_cast<float>(cos(angle));
        float z = static_cast<float>(sin(angle));
        vertices.push_back(x);
        vertices.push_back(1.0f);
        vertices.push_back(z);
    }

    for (int i = 1; i <= segments; i++) {
        int next = (i % segments) + 1;
        index.push_back(0);
        index.push_back(static_cast<unsigned int>(next));
        index.push_back(static_cast<unsigned int>(i));
    }

    int topCenter = segments + 1;
    for (int i = 1; i <= segments; i++) {
        int next = (i % segments) + 1 + segments + 1;
        index.push_back(static_cast<unsigned int>(topCenter));
        index.push_back(static_cast<unsigned int>(i + segments + 1));
        index.push_back(static_cast<unsigned int>(next));
    }

    for (int i = 1; i <= segments; i++) {
        int next = (i % segments) + 1;

        int bottomCurrent = i;
        int bottomNext = next;
        int topCurrent = i + segments + 1;
        int topNext = next + segments + 1;

        index.push_back(static_cast<unsigned int>(bottomCurrent));
        index.push_back(static_cast<unsigned int>(topCurrent));
        index.push_back(static_cast<unsigned int>(bottomNext));

        index.push_back(static_cast<unsigned int>(bottomNext));
        index.push_back(static_cast<unsigned int>(topCurrent));
        index.push_back(static_cast<unsigned int>(topNext));
    }
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}

void Mesh::newPyramid(const gr::Color3 color){
    std::vector<float> vertices = {
        1.0f, -1.0f,  1.0f, // 0: base front right
       -1.0f, -1.0f,  1.0f, // 1: base front left
        1.0f, -1.0f, -1.0f, // 2: base back right
       -1.0f, -1.0f, -1.0f, // 3: base back left
        0.0f,  1.0f,  0.0f  // 4: top
    };

    std::vector<unsigned int> index = {
        0, 2, 1,
        2, 3, 1,
        0, 1, 4,
        1, 3, 4,
        3, 2, 4,
        2, 0, 4
    };
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
}

void Mesh::newCone(const gr::Color3 color, int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.clear();
    index.clear();

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i < segments; i++){
        float angle = static_cast<float>(i / segments) * 2.0f * gr::Math::PI;
        float x = static_cast<float>(cos(angle));
        float z = static_cast<float>(sin(angle));

        vertices.push_back(x);
        vertices.push_back(-1.0f);
        vertices.push_back(z);
    }

    for (int i = 1; i < segments - 1; i++){
        index.push_back(1);
        index.push_back(static_cast<unsigned int>(i + 1));
        index.push_back(static_cast<unsigned int>(i + 2));
    }

    for (int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;

        index.push_back(0);
        index.push_back(static_cast<unsigned int>(i));
        index.push_back(static_cast<unsigned int>(next));
    }
    std::vector<float> colors = {
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b),
         static_cast<float>(color.r), static_cast<float>(color.g), static_cast<float>(color.b)
    };
    this->upload(vertices, index, colors);
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