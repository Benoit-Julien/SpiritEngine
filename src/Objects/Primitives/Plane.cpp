#include "Plane.hpp"

Plane::Plane() : TriangleObject(ObjectType::Plane) {
	this->setupVertices();
	this->setup();
	this->name = "Plane";
}

Plane::~Plane() {}

Plane::Plane(const Plane &plane) : TriangleObject(plane) {}

Plane &Plane::operator=(const Plane &plane) {
	return *this;
}

void Plane::setupVertices() {
	this->vertices = std::vector<glm::vec3>{
					glm::vec3(-1.0f, 0, 1.0f),
					glm::vec3(1.0f, 0, 1.0f),
					glm::vec3(1.0f, 0, -1.0f),
					glm::vec3(-1.0f, 0, -1.0f)
	};

	glm::vec3 up(0, 1, 0);
	this->normals = std::vector<glm::vec3>{
					up,
					up,
					up,
					up
	};

	this->triangles = std::vector<TriangleIndex>{
					{0, 1, 2},
					{0, 2, 3}
	};
}