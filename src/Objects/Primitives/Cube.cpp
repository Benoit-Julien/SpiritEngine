#include "Cube.hpp"

Cube::Cube() : TriangleObject(ObjectType::Cube) {
	this->setupVertices();
	this->setup();
	this->name = "Cube";
}

Cube::~Cube() {}

Cube::Cube(const Cube &cube) : TriangleObject(cube) {}

Cube &Cube::operator=(const Cube &cube) {
	return *this;
}

void Cube::setupVertices() {

	/************ Vertices ************/
	glm::vec3 p0(-1, -1, 1);
	glm::vec3 p1(1, -1, 1);
	glm::vec3 p2(1, -1, -1);
	glm::vec3 p3(-1, -1, -1);

	glm::vec3 p4(-1, 1, 1);
	glm::vec3 p5(1, 1, 1);
	glm::vec3 p6(1, 1, -1);
	glm::vec3 p7(-1, 1, -1);

	this->vertices = std::vector<glm::vec3>{
					// Bottom
					p0, p1, p2, p3,

					// Left
					p7, p4, p0, p3,

					// Front
					p4, p5, p1, p0,

					// Back
					p6, p7, p3, p2,

					// Right
					p5, p6, p2, p1,

					// Top
					p7, p6, p5, p4
	};

	/************ Normals ************/
	glm::vec3 up(0, 1, 0);
	glm::vec3 down(0, -1, 0);
	glm::vec3 front(0, 0, 1);
	glm::vec3 back(0, 0, -1);
	glm::vec3 left(-1, 0, 0);
	glm::vec3 right(1, 0, 0);

	this->normals = std::vector<glm::vec3>{
					// Bottom
					down, down, down, down,

					// Left
					left, left, left, left,

					// Front
					front, front, front, front,

					// Back
					back, back, back, back,

					// Right
					right, right, right, right,

					// Top
					up, up, up, up
	};

	/************ Triangles ************/
	this->triangles = std::vector<TriangleIndex>{
					// Bottom
					{3,         1,         0},
					{3,         2,         1},

					// Left
					{3 + 4 * 1, 1 + 4 * 1, 0 + 4 * 1},
					{3 + 4 * 1, 2 + 4 * 1, 1 + 4 * 1},

					// Front
					{3 + 4 * 2, 1 + 4 * 2, 0 + 4 * 2},
					{3 + 4 * 2, 2 + 4 * 2, 1 + 4 * 2},

					// Back
					{3 + 4 * 3, 1 + 4 * 3, 0 + 4 * 3},
					{3 + 4 * 3, 2 + 4 * 3, 1 + 4 * 3},

					// Right
					{3 + 4 * 4, 1 + 4 * 4, 0 + 4 * 4},
					{3 + 4 * 4, 2 + 4 * 4, 1 + 4 * 4},

					// Top
					{3 + 4 * 5, 1 + 4 * 5, 0 + 4 * 5},
					{3 + 4 * 5, 2 + 4 * 5, 1 + 4 * 5}
	};

	/************ UVs ************/
	glm::vec2 _00(0.0f, 0.0f);
	glm::vec2 _10(1.0f, 0.0f);
	glm::vec2 _01(0.0f, 1.0f);
	glm::vec2 _11(1.0f, 1.0f);

	this->uvs = std::vector<glm::vec2>{
					// Bottom
					_11, _01, _00, _10,

					// Left
					_11, _01, _00, _10,

					// Front
					_11, _01, _00, _10,

					// Back
					_11, _01, _00, _10,

					// Right
					_11, _01, _00, _10,

					// Top
					_11, _01, _00, _10,
	};
}