#include "WireCube.hpp"

WireCube::WireCube() : TriangleObject(ObjectType::WireCube) {
	this->setupVertices();
	this->setup();
	this->setDrawMode(DrawMode::WireFrame);
}

WireCube::~WireCube() {}

void WireCube::setupVertices() {
	this->vertices = std::vector<glm::vec3>{
					// front
					glm::vec3(-1.0, -1.0, 1.0),
					glm::vec3(1.0, -1.0, 1.0),
					glm::vec3(1.0, 1.0, 1.0),
					glm::vec3(-1.0, 1.0, 1.0),
					// back
					glm::vec3(-1.0, -1.0, -1.0),
					glm::vec3(1.0, -1.0, -1.0),
					glm::vec3(1.0, 1.0, -1.0),
					glm::vec3(-1.0, 1.0, -1.0)
	};

	this->colors = std::vector<glm::vec4>{
					// front
					glm::vec4(0, 0, 0, 1.0),
					glm::vec4(0, 0, 0, 1.0),
					glm::vec4(0, 0, 0, 1.0),
					glm::vec4(0, 0, 0, 1.0),
					// back
					glm::vec4(0, 0, 0, 1.0),
					glm::vec4(0, 0, 0, 1.0),
					glm::vec4(0, 0, 0, 1.0),
					glm::vec4(0, 0, 0, 1.0),
	};

	this->triangles = std::vector<TriangleIndex>{
					{0, 1, 2},
					{2, 3, 0},
					{1, 5, 6},
					{6, 2, 1},
					{7, 6, 5},
					{5, 4, 7},
					{4, 0, 3},
					{3, 7, 4},
					{4, 5, 1},
					{1, 0, 4},
					{3, 2, 6},
					{6, 7, 3}
	};
}