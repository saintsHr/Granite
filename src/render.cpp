#include "granite/render.hpp"
#include "granite/math.hpp"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace gr::internal {

    static const char* defaultVertex = R"(

    #version 330 core
    layout(location = 0) in vec3 aPos;

    uniform mat4 uProjection;
    uniform mat4 uView;
    uniform mat4 uModel;

    void main(){
        gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    }

    )";

    static const char* defaultFragment = R"(

    #version 330 core
    out vec4 FragColor;

    uniform vec4 uColor;

    void main(){
        FragColor = uColor;
    }

    )";
}

namespace gr::Render{

void init(){
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return;
    glEnable(GL_DEPTH_TEST);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
}

void Mesh::upload(const std::vector<float>& vertices){
    vertexCount_ = vertices.size() / 3;
    indexCount_ = 0;

    // bind buffers
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    // fills VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // configs VAO
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Mesh::upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index){
    vertexCount_ = vertices.size() / 3;
    indexCount_ = index.size();

    // bind buffers
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    // fills VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(float),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // fills EBO data
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        index.size() * sizeof(unsigned int),
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
        (void*)0
    );

    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Mesh::draw(Shader shader, gr::Color3 color, GLenum drawMode) const{
    shader.use();

    glBindVertexArray(vao_);

    // checks if color location exists, if not, find it
    GLint cLoc;
    if (cL_ == 0){
        cLoc = glGetUniformLocation(shader.getProgram(), "uColor");
    } else {
        cLoc = cL_;
    }

    // applies color if valid location
    if (cLoc != -1){
        glUniform4f(
            cLoc,
            color.r * (1.0f / 255.0f),
            color.g * (1.0f / 255.0f),
            color.b * (1.0f / 255.0f),
            1.0f
        );
    }

    // draws the mesh
    if(indexCount_ > 0){
        glDrawElements(drawMode, indexCount_, GL_UNSIGNED_INT, 0);
    }else{
        glDrawArrays(drawMode, 0, vertexCount_);
    }

    glBindVertexArray(0);
}

void Mesh::newTriangle(){
    std::vector<float> vertices = {
        -1.0f, -0.866f, 0.0f,
         1.0f, -0.866f, 0.0f,
         0.0f,  0.866f, 0.0f,
    };
    this->upload(vertices);
}
void Mesh::newQuad(){
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
    this->upload(vertices, index);
}

void Mesh::newCube(){
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
    this->upload(vertices, index);
}

void Mesh::newCircle(int segments) {
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);

    for(int i = 0; i < segments; i++){
        float angle = 2.0f * gr::Math::PI * i / segments;
        vertices.push_back(cos(angle));
        vertices.push_back(sin(angle));
        vertices.push_back(0.0f);
    }

    for(int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;
        index.push_back(0);
        index.push_back(i);
        index.push_back(next);
    }

    this->upload(vertices, index);
}

void Mesh::newSphere(int latSegments, int longSegments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    for(int lat = 0; lat <= latSegments; lat++){
        float theta = lat * gr::Math::PI / latSegments;
        float sinTheta = sin(theta);
        float cosTheta = cos(theta);

        for(int lon = 0; lon <= longSegments; lon++){
            float phi = lon * 2.0f * gr::Math::PI / longSegments;
            float sinPhi = sin(phi);
            float cosPhi = cos(phi);

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

            index.push_back(first);
            index.push_back(second);
            index.push_back(first + 1);

            index.push_back(second);
            index.push_back(second + 1);
            index.push_back(first + 1);
        }
    }

    this->upload(vertices, index);
}

void Mesh::newCylinder(int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.push_back(0.0f); vertices.push_back(-1.0f); vertices.push_back(0.0f);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.14159265f * i / segments;
        float x = cos(angle);
        float z = sin(angle);
        vertices.push_back(x);
        vertices.push_back(-1.0f);
        vertices.push_back(z);
    }

    vertices.push_back(0.0f); vertices.push_back(1.0f); vertices.push_back(0.0f);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.14159265f * i / segments;
        float x = cos(angle);
        float z = sin(angle);
        vertices.push_back(x);
        vertices.push_back(1.0f);
        vertices.push_back(z);
    }

    for (int i = 1; i <= segments; i++) {
        int next = (i % segments) + 1;
        index.push_back(0);
        index.push_back(next);
        index.push_back(i);
    }

    int topCenter = segments + 1;
    for (int i = 1; i <= segments; i++) {
        int next = (i % segments) + 1 + segments + 1;
        index.push_back(topCenter);
        index.push_back(i + segments + 1);
        index.push_back(next);
    }

    for (int i = 1; i <= segments; i++) {
        int next = (i % segments) + 1;

        int bottomCurrent = i;
        int bottomNext = next;
        int topCurrent = i + segments + 1;
        int topNext = next + segments + 1;

        index.push_back(bottomCurrent);
        index.push_back(topCurrent);
        index.push_back(bottomNext);

        index.push_back(bottomNext);
        index.push_back(topCurrent);
        index.push_back(topNext);
    }

    this->upload(vertices, index);
}

void Mesh::newPyramid(){
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

    this->upload(vertices, index);
}

void Mesh::newCone(int segments){
    std::vector<float> vertices;
    std::vector<unsigned int> index;

    vertices.clear();
    index.clear();

    vertices.push_back(0.0f);
    vertices.push_back(1.0f);
    vertices.push_back(0.0f);

    for (int i = 0; i < segments; i++){
        float angle = (float)i / segments * 2.0f * gr::Math::PI;
        float x = cos(angle);
        float z = sin(angle);

        vertices.push_back(x);
        vertices.push_back(-1.0f);
        vertices.push_back(z);
    }

    for (int i = 1; i < segments - 1; i++){
        index.push_back(1);
        index.push_back(i + 1);
        index.push_back(i + 2);
    }

    for (int i = 1; i <= segments; i++){
        int next = (i % segments) + 1;

        index.push_back(0);
        index.push_back(i);
        index.push_back(next);
    }

    this->upload(vertices, index);
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

void RenderObject::draw(const Shader& shader, gr::Color3 color, GLenum drawMode) const{
    shader.use();

    // calculates transform
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(transform.position.x, transform.position.y, transform.position.z));
    model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

    // checks if model location exists, if not, find it
    GLint mLoc;
    if (mL_ == 0){
        mLoc = glGetUniformLocation(shader.getProgram(), "uModel");
    } else {
        mLoc = mL_;
    }

    // applies transform if valid location
    if (mLoc != -1){
        glUniformMatrix4fv(
            mLoc,
            1,
            GL_FALSE,
            &model[0][0]
        );
    }

    // draws mesh
    mesh->draw(shader, color, drawMode);
}

Shader::Shader(const char* vertexSource, const char* fragmentSource){
    // creates & compiles shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // links shaders into program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    program_ = shaderProgram;

    // delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::Shader() : Shader(gr::internal::defaultVertex, gr::internal::defaultFragment){}

Shader::~Shader(){
    glDeleteProgram(program_);
}

void Shader::use() const{
    glUseProgram(program_);
}

GLuint Shader::getProgram() const{
    return program_;
}

};