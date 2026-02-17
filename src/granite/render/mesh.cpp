#include "granite/render/mesh.hpp"
#include "granite/core/math.hpp"

#include <glm/glm.hpp>
#include <cmath>

namespace gr::Render {

void Mesh::upload(const std::vector<float>& vertices, const std::vector<unsigned int>& index, const std::vector<float>& normals, const std::vector<float>& uvs) {
    size_t numVertices = vertices.size() / 3;
    std::vector<float> interleaved;
    interleaved.reserve(numVertices * 8);

    for (size_t i = 0; i < numVertices; ++i){
        interleaved.push_back(vertices[i*3 + 0]);
        interleaved.push_back(vertices[i*3 + 1]);
        interleaved.push_back(vertices[i*3 + 2]);

        interleaved.push_back(normals[i*3 + 0]);
        interleaved.push_back(normals[i*3 + 1]);
        interleaved.push_back(normals[i*3 + 2]);

        interleaved.push_back(uvs[i*2 + 0]);
        interleaved.push_back(uvs[i*2 + 1]);
    }

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    // VBO data
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(interleaved.size() * static_cast<long int>(sizeof(float))),
        interleaved.data(),
        GL_STATIC_DRAW
    );

    // EBO data
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(index.size() * static_cast<long int>(sizeof(unsigned int))),
        index.data(),
        GL_STATIC_DRAW
    );

    // configs VAO
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        nullptr
    );
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        reinterpret_cast<void*>(3*sizeof(float))
    );
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        reinterpret_cast<void*>(6*sizeof(float))
    );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    vertexCount_ = static_cast<unsigned int>(numVertices);
    indexCount_  = static_cast<unsigned int>(index.size());
}

void Mesh::draw(const Shader& shader) const {
    shader.use();

    glBindVertexArray(vao_);

    // draws the mesh
    if(indexCount_ > 0){
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, nullptr);
    }else{
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount_));
    }

    glBindVertexArray(0);
}

void Mesh::newTriangle() {
    std::vector<float> vertices = {
       -1.0f, -0.866f, 0.0f,
        1.0f, -0.866f, 0.0f,
        0.0f,  0.866f, 0.0f,

       -1.0f, -0.866f, 0.0f,
        0.0f,  0.866f, 0.0f,
        1.0f, -0.866f, 0.0f
    };

    std::vector<float> normals = {
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,
        0.0f, 0.0f,  1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    };

    std::vector<float> uvs = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.5f, 0.0f,

        0.0f, 1.0f,
        0.5f, 0.0f,
        1.0f, 1.0f
    };

    std::vector<unsigned int> index = {
        0, 1, 2,
        3, 4, 5
    };

    upload(vertices, index, normals, uvs);
}

void Mesh::newQuad() {
    std::vector<float> vertices = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,

        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    std::vector<float> normals = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    };

    std::vector<unsigned int> index = {
        0, 1, 2,
        0, 2, 3,

        4, 6, 5,
        4, 7, 6
    };

    std::vector<float> uvs = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    upload(vertices, index, normals, uvs);
}

void Mesh::newCube() {
    std::vector<float> vertices = {
       -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
       -1.0f,-1.0f,-1.0f,
       -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
       -1.0f,-1.0f,-1.0f,
       -1.0f,-1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
       -1.0f, 1.0f,-1.0f,
       -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
       -1.0f,-1.0f, 1.0f
    };

    std::vector<float> normals = {
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f,
        0.0f, -1.0f,  0.0f
    };

    std::vector<unsigned int> index = {
        0,  1,   2,
        0,  2,   3,
        4,  5,   6,
        4,  6,   7,
        8,  9,  10,
        8,  10, 11,
        12, 13, 14,
        12, 14, 15,
        16, 17, 18,
        16, 18, 19,
        20, 21, 22,
        20, 22, 23
    };

    std::vector<float> uvs = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    upload(vertices, index, normals, uvs);
}

void Mesh::newCircle(int segments) {
	std::vector<float> vertices;
	std::vector<unsigned int> index;
	std::vector<float> normals;
	std::vector<float> uvs;

	const float radius = 1.0f;

	// center
	vertices.insert(vertices.end(), {0.f, 0.f, 0.f});
	normals.insert(normals.end(), {0.f, 0.f, 1.f});
	uvs.insert(uvs.end(), {0.5f, 0.5f});

	// borders
	for (int i = 0; i <= segments; i++) {
		float angle = (float)i * 2.f * gr::Math::PI / (float)segments;

		float x = std::cos(angle) * radius;
		float y = std::sin(angle) * radius;

		vertices.insert(vertices.end(), {x, y, 0.f});
		normals.insert(normals.end(), {0.f, 0.f, 1.f});

		// UV mapeado para [0,1]
		float u = (x / radius + 1.f) * 0.5f;
		float v = (y / radius + 1.f) * 0.5f;
		uvs.insert(uvs.end(), {u, v});
	}

	// front indexes
	for (int i = 1; i <= segments; i++) {
		index.insert(index.end(), {
			0,
			(unsigned int)i,
			(unsigned int)(i + 1)
		});
	}

	unsigned int baseIndex = vertices.size() / 3;

	// center (back)
	vertices.insert(vertices.end(), {0.f, 0.f, 0.f});
	normals.insert(normals.end(), {0.f, 0.f, -1.f});
	uvs.insert(uvs.end(), {0.5f, 0.5f});

	// borders (back)
	for (int i = 0; i <= segments; i++) {
		float angle = (float)i * 2.f * gr::Math::PI / (float)segments;

		float x = std::cos(angle) * radius;
		float y = std::sin(angle) * radius;

		vertices.insert(vertices.end(), {x, y, 0.f});
		normals.insert(normals.end(), {0.f, 0.f, -1.f});

		float u = (x / radius + 1.f) * 0.5f;
		float v = (y / radius + 1.f) * 0.5f;
		uvs.insert(uvs.end(), {u, v});
	}

	// back index
	for (int i = 1; i <= segments; i++) {
		index.insert(index.end(), {
			baseIndex,
			baseIndex + i + 1,
			baseIndex + i
		});
	}

	upload(vertices, index, normals, uvs);
}

void Mesh::newSphere(int latSeg, int lonSeg) {
	std::vector<float> vertices;
	std::vector<unsigned int> index;
	std::vector<float> normals;
	std::vector<float> uvs;

	for (int lat = 0; lat <= latSeg; lat++) {
		float theta = static_cast<float>(lat) * gr::Math::PI / static_cast<float>(latSeg);
		float v = static_cast<float>(lat) / static_cast<float>(latSeg);

		for (int lon = 0; lon <= lonSeg; lon++){
			float phi = static_cast<float>(lon) * 2.f * gr::Math::PI / static_cast<float>(lonSeg);
			float u = static_cast<float>(lon) / static_cast<float>(lonSeg);

			float x = std::sin(theta) * std::cos(phi);
			float y = std::cos(theta);
			float z = std::sin(theta) * std::sin(phi);

			vertices.insert(vertices.end(), {x, y, z});
			normals.insert(normals.end(), {x, y, z});
			uvs.insert(uvs.end(), {1.0f - u, v});
		}
	}

	for (int lat = 0; lat < latSeg; lat++) {
		for (int lon = 0; lon < lonSeg; lon++){
			int a = lat * (lonSeg + 1) + lon;
			int b = a + lonSeg + 1;

			index.insert(index.end(), {
				static_cast<unsigned>(a), static_cast<unsigned>(a+1), static_cast<unsigned>(b),
				static_cast<unsigned>(a + 1), static_cast<unsigned>(b + 1), static_cast<unsigned>(b)
			});
		}
	}

	upload(vertices, index, normals, uvs);
}

void Mesh::newCylinder(int segments) {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> index;
	std::vector<float> uvs;

	const float yBottom = -1.f;
	const float yTop    =  1.f;

	unsigned int baseCenter = 0;
	vertices.insert(vertices.end(), {0.f, yBottom, 0.f});
	normals.insert(normals.end(),  {0.f, -1.f, 0.f});
	uvs.insert(uvs.end(), {0.5f, 0.5f});

	unsigned int baseRingStart = static_cast<unsigned int>(vertices.size()) / 3;

	for (int i = 0; i < segments; i++) {
		float a = 2.f * gr::Math::PI * float(i) / float(segments);
		float x = std::cos(a);
		float z = std::sin(a);

		vertices.insert(vertices.end(), {x, yBottom, z});
		normals.insert(normals.end(),  {0.f, -1.f, 0.f});
		uvs.insert(uvs.end(), {x * 0.5f + 0.5f, z * 0.5f + 0.5f});
	}

	for (int i = 0; i < segments; i++) {
		unsigned int next = (static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(segments);
		index.insert(index.end(), {
			baseCenter,
			baseRingStart + static_cast<unsigned int>(i),
			baseRingStart + next
		});
	}

	unsigned int topCenter = static_cast<unsigned int>(vertices.size()) / 3;
	vertices.insert(vertices.end(), {0.f, yTop, 0.f});
	normals.insert(normals.end(),  {0.f, 1.f, 0.f});
	uvs.insert(uvs.end(), {0.5f, 0.5f});

	unsigned int topRingStart = static_cast<unsigned int>(vertices.size()) / 3;

	for (int i = 0; i < segments; i++) {
		float a = 2.f * gr::Math::PI * float(i) / float(segments);
		float x = std::cos(a);
		float z = std::sin(a);

		vertices.insert(vertices.end(), {x, yTop, z});
		normals.insert(normals.end(),  {0.f, 1.f, 0.f});
		uvs.insert(uvs.end(), {x * 0.5f + 0.5f, z * 0.5f + 0.5f});
	}

	for (int i = 0; i < segments; i++) {
		unsigned int next = (static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(segments);
		index.insert(index.end(), {
			topCenter,
			topRingStart + next,
			topRingStart + static_cast<unsigned int>(i)
		});
	}

	unsigned int sideStart = static_cast<unsigned int>(vertices.size()) / 3;

    for (int i = 0; i <= segments; i++) {
        float a = 2.f * gr::Math::PI * float(i) / float(segments);
        float x = std::cos(a);
        float z = std::sin(a);

        float u = 1.f - static_cast<float>(i) / static_cast<float>(segments);
        float vBottom = 1.f;
        float vTop    = 0.f;

        vertices.insert(vertices.end(), {x, yBottom, z});
        normals.insert(normals.end(), {x, 0.f, z});
        uvs.insert(uvs.end(), {u, vBottom});

        vertices.insert(vertices.end(), {x, yTop, z});
        normals.insert(normals.end(), {x, 0.f, z});
        uvs.insert(uvs.end(), {u, vTop});
    }

	for (int i = 0; i < segments; i++) {
		unsigned int next = (static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(segments);

		unsigned int b0 = sideStart + static_cast<unsigned int>(i) * 2;
		unsigned int t0 = b0 + 1;
		unsigned int b1 = sideStart + next * 2;
		unsigned int t1 = b1 + 1;

		index.insert(index.end(), {
			b0, t0, b1,
			b1, t0, t1
		});
	}

	upload(vertices, index, normals, uvs);
}

void Mesh::newPyramid() {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<unsigned int> index;

    unsigned int baseStart = 0;

    vertices.insert(vertices.end(), {
         1.f, -1.f,  1.f,
        -1.f, -1.f,  1.f,
        -1.f, -1.f, -1.f,
         1.f, -1.f, -1.f
    });

    for (int i = 0; i < 4; i++) normals.insert(normals.end(), {0.f, -1.f, 0.f});

    index.insert(index.end(), {
        baseStart + 0, baseStart + 1, baseStart + 2,
        baseStart + 0, baseStart + 2, baseStart + 3
    });


    glm::vec3 apex(0.f, 1.f, 0.f);

    {
        glm::vec3 a( 1.f, -1.f,  1.f);
        glm::vec3 b(-1.f, -1.f,  1.f);

        glm::vec3 n = glm::normalize(glm::cross(b - apex, a - apex));

        unsigned int start = static_cast<unsigned int>(vertices.size()) / 3;

        vertices.insert(vertices.end(), {
            a.x, a.y, a.z,
            b.x, b.y, b.z,
            apex.x, apex.y, apex.z
        });

        for (int i = 0; i < 3; i++) normals.insert(normals.end(), {n.x, n.y, n.z});

        index.insert(index.end(), {start, start + 2, start + 1});
    }

    {
        glm::vec3 a(-1.f, -1.f,  1.f);
        glm::vec3 b(-1.f, -1.f, -1.f);

        glm::vec3 n = glm::normalize(glm::cross(b - apex, a - apex));

        unsigned int start = static_cast<unsigned int>(vertices.size()) / 3;

        vertices.insert(vertices.end(), {
            a.x, a.y, a.z,
            b.x, b.y, b.z,
            apex.x, apex.y, apex.z
        });

        for (int i = 0; i < 3; i++) normals.insert(normals.end(), {n.x, n.y, n.z});

        index.insert(index.end(), {start, start + 2, start + 1});
    }

    {
        glm::vec3 a(-1.f, -1.f, -1.f);
        glm::vec3 b( 1.f, -1.f, -1.f);

        glm::vec3 n = glm::normalize(glm::cross(b - apex, a - apex));

        unsigned int start = static_cast<unsigned int>(vertices.size() )/ 3;

        vertices.insert(vertices.end(), {
            a.x, a.y, a.z,
            b.x, b.y, b.z,
            apex.x, apex.y, apex.z
        });

        for (int i = 0; i < 3; i++) normals.insert(normals.end(), {n.x, n.y, n.z});

        index.insert(index.end(), {start, start + 2, start + 1});
    }

    {
        glm::vec3 a( 1.f, -1.f, -1.f);
        glm::vec3 b( 1.f, -1.f,  1.f);

        glm::vec3 n = glm::normalize(glm::cross(b - apex, a - apex));

        unsigned int start = static_cast<unsigned int>(vertices.size()) / 3;

        vertices.insert(vertices.end(), {
            a.x, a.y, a.z,
            b.x, b.y, b.z,
            apex.x, apex.y, apex.z
        });

        for (int i = 0; i < 3; i++) normals.insert(normals.end(), {n.x, n.y, n.z});

        index.insert(index.end(), {start, start + 2, start + 1});
    }

    std::vector<float> uvs = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.5f, 0.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.5f, 0.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.5f, 0.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.5f, 0.0f
    };

    upload(vertices, index, normals, uvs);
}

void Mesh::newCone(int segments) {
	std::vector<float> vertices;
	std::vector<float> normals;
	std::vector<unsigned int> index;
	std::vector<float> uvs;

	const float yTop    =  1.f;
	const float yBottom = -1.f;
	const float slope   = 0.5f;

	unsigned int baseCenter = 0;
	vertices.insert(vertices.end(), {0.f, yBottom, 0.f});
	normals.insert(normals.end(),  {0.f, -1.f, 0.f});
	uvs.insert(uvs.end(), {0.5f, 0.5f});

	unsigned int baseRingStart = static_cast<unsigned int>(vertices.size()) / 3;

	for (int i = 0; i < segments; i++) {
		float a = 2.f * gr::Math::PI * float(i) / float(segments);
		float x = std::cos(a);
		float z = std::sin(a);

		vertices.insert(vertices.end(), {x, yBottom, z});
		normals.insert(normals.end(),  {0.f, -1.f, 0.f});
		float u = (x + 1.f) * 0.5f;
		float v = (1.f - z) * 0.5f;
		uvs.insert(uvs.end(), {u, v});
	}

	for (int i = 0; i < segments; i++) {
		unsigned int next = (static_cast<unsigned int>(i) + 1) % static_cast<unsigned int>(segments);
		index.insert(index.end(), {
			baseCenter,
			baseRingStart + static_cast<unsigned int>(i),
			baseRingStart + static_cast<unsigned int>(next)
		});
	}

	unsigned int sideStart = static_cast<unsigned int>(vertices.size()) / 3;
	for (int i = 0; i <= segments; i++) {
		float a = 2.f * gr::Math::PI * float(i) / float(segments);
		float x = std::cos(a);
		float z = std::sin(a);

		float u = 1.f - static_cast<float>(i) / static_cast<float>(segments);
		float vBottom = 1.f;
		float vTop    = 0.f;

		vertices.insert(vertices.end(), {x, yBottom, z});
		glm::vec3 n = glm::normalize(glm::vec3(x, slope, z));
		normals.insert(normals.end(), {n.x, n.y, n.z});
		uvs.insert(uvs.end(), {u, vBottom});

		vertices.insert(vertices.end(), {0.f, yTop, 0.f});
		normals.insert(normals.end(), {n.x, n.y, n.z});
		uvs.insert(uvs.end(), {u, vTop});
	}

	for (int i = 0; i < segments; i++) {
		unsigned int b0 = sideStart + static_cast<unsigned int>(i) * 2;
		unsigned int t0 = b0 + 1;
		unsigned int b1 = sideStart + (static_cast<unsigned int>(i) + 1) * 2;
		unsigned int t1 = b1 + 1;

		index.insert(index.end(), {
			b0, t0, b1,
			b1, t0, t1
		});
	}

	upload(vertices, index, normals, uvs);
}

Mesh::~Mesh() {
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
}

Mesh::Mesh() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
}
    
}