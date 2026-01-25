#include "granite/render/mesh.hpp"
#include "granite/core/math.hpp"

#include <cmath>

namespace gr::Render{

void Mesh::upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index){
    vertexCount_ = static_cast<uint32_t>(vertices.size() / 3);
    indexCount_ = static_cast<uint32_t>(index.size());

    GLsizeiptr vertSize  = static_cast<GLsizeiptr>(vertices.size() * sizeof(float));

    // bind buffers
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    // fills VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertSize,
        vertices.data(),
        GL_STATIC_DRAW
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
        3 * sizeof(float),
        static_cast<void*>(0)
    );

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Mesh::draw(const Shader& shader, GLenum drawMode) const{
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

void Mesh::newTriangle(){
    std::vector<float> vertices = {
        -1.0f, -0.866f, 0.0f,
         1.0f, -0.866f, 0.0f,
         0.0f,  0.866f, 0.0f
    };

    std::vector<unsigned int> index = {
        0, 1, 2
    };

    upload(vertices, index);
}

void Mesh::newQuad(){
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

    upload(vertices, index);
}

void Mesh::newCube(){
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

    upload(vertices, index);
}

void Mesh::newCircle(int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.insert(vertices.end(), {0.f, 0.f, 0.f});

    for (int i = 0; i < segments; i++){
        float angle = 2.0f * gr::Math::PI * float(static_cast<float>(i) / static_cast<float>(segments));
        vertices.push_back(static_cast<float>(cos(static_cast<float>(angle))));
        vertices.push_back(static_cast<float>(sin(static_cast<float>(angle))));
        vertices.push_back(0.f);
    }

    for (int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;
        index.insert(index.end(), {0, static_cast<unsigned>(i), static_cast<unsigned>(next)});
    }

    upload(vertices, index);
}

void Mesh::newSphere(int latSeg, int lonSeg){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    for (int lat = 0; lat <= latSeg; lat++){
        float theta = static_cast<float>(lat) * gr::Math::PI / static_cast<float>(latSeg);
        for (int lon = 0; lon <= lonSeg; lon++){
            float phi = static_cast<float>(lon) * 2.f * gr::Math::PI / static_cast<float>(lonSeg);

            float x = static_cast<float>(sin(static_cast<float>(theta))) * static_cast<float>(cos(static_cast<float>(phi)));
            float y = static_cast<float>(cos(static_cast<float>(theta)));
            float z = static_cast<float>(sin(static_cast<float>(theta))) * static_cast<float>(sin(static_cast<float>(phi)));

            vertices.insert(vertices.end(), {x,y,z});
        }
    }

    for (int lat = 0; lat < latSeg; lat++){
        for (int lon = 0; lon < lonSeg; lon++){
            int a = lat * (lonSeg + 1) + lon;
            int b = a + lonSeg + 1;

            index.insert(index.end(), {
                static_cast<unsigned>(a), static_cast<unsigned>(b), static_cast<unsigned>(a + 1),
                static_cast<unsigned>(b), static_cast<unsigned>(b+1), static_cast<unsigned>(a+1)
            });
        }
    }

    upload(vertices, index);
}

void Mesh::newCylinder(int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.insert(vertices.end(), {0.f, -1.f, 0.f});

    for(int i = 0; i < segments; i++){
        float a = 2.f * gr::Math::PI * (static_cast<float>(i) / static_cast<float>(segments));
        float x = static_cast<float>(cos(static_cast<float>(a)));
        float z = static_cast<float>(sin(static_cast<float>(a)));

        vertices.insert(vertices.end(), {x, -1.f, z});
    }

    long unsigned int topCenter = vertices.size() / 3;
    vertices.insert(vertices.end(), {0.f, 1.f, 0.f});

    long unsigned int topRingStart = vertices.size() / 3;
    for(int i = 0; i < segments; i++){
        float a = 2.f * gr::Math::PI * (static_cast<float>(i) / static_cast<float>(segments));
        float x = static_cast<float>(cos(static_cast<float>(a)));
        float z = static_cast<float>(sin(static_cast<float>(a)));

        vertices.insert(vertices.end(), {x, 1.f, z});
    }

    for(int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;
        index.insert(index.end(), {0u, static_cast<unsigned>(next), static_cast<unsigned>(i)});
    }

    for(int i = 0; i < segments; i++){
        int curr = static_cast<int>(topRingStart) + i;
        int next = static_cast<int>(topRingStart) + (i + 1) % segments;
        index.insert(index.end(), {
            static_cast<unsigned>(topCenter),
            static_cast<unsigned>(curr),
            static_cast<unsigned>(next)
        });
    }

    for(int i = 0; i < segments; i++){
        int b0 = 1 + i;
        int b1 = 1 + (i + 1) % segments;
        int t0 = static_cast<int>(topRingStart) + i;
        int t1 = static_cast<int>(topRingStart) + (i + 1) % segments;

        index.insert(index.end(), {
            static_cast<unsigned>(b0), static_cast<unsigned>(t0), static_cast<unsigned>(b1),
            static_cast<unsigned>(b1), static_cast<unsigned>(t0), static_cast<unsigned>(t1)
        });
    }

    upload(vertices, index);
}

void Mesh::newPyramid(){
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

    upload(vertices, index);
}

void Mesh::newCone(int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.insert(vertices.end(), {0.f, 1.f, 0.f});

    long unsigned int baseCenter = vertices.size() / 3;
    vertices.insert(vertices.end(), {0.f, -1.f, 0.f});

    long unsigned int ringStart = vertices.size() / 3;
    for(int i = 0; i < segments; i++){
        float a = 2.f * gr::Math::PI * (static_cast<float>(i) / static_cast<float>(segments));
        float x = static_cast<float>(cos(static_cast<float>(a)));
        float z = static_cast<float>(sin(static_cast<float>(a)));

        vertices.insert(vertices.end(), {x, -1.f, z});
    }

    for(int i = 0; i < segments; i++){
        int curr = static_cast<int>(ringStart) + i;
        int next = static_cast<int>(ringStart) + (i + 1) % segments;
        index.insert(index.end(), {
            0u,
            static_cast<unsigned>(curr),
            static_cast<unsigned>(next)
        });
    }

    for(int i = 0; i < segments; i++){
        int curr = static_cast<int>(ringStart) + i;
        int next = static_cast<int>(ringStart) + (i + 1) % segments;
        index.insert(index.end(), {
            static_cast<unsigned>(baseCenter),
            static_cast<unsigned>(next),
            static_cast<unsigned>(curr)
        });
    }

    upload(vertices, index);
}

Mesh::~Mesh(){
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

Mesh::Mesh(){
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}
    
}