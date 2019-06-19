#include "SkyBox.hpp"

SkyBox::SkyBox() : TriangleObject(ObjectType::SkyBox) {
	this->setupVertices();
	this->setup();
	this->name = "SkyBox";

	this->LockTranslate();
	this->LockRotate();
	this->LockScale();
}

SkyBox::~SkyBox() {}

SkyBox::SkyBox(const SkyBox &skyBox) : TriangleObject(skyBox) {}

SkyBox& SkyBox::operator=(const SkyBox &skyBox) {
	return *this;
}

void SkyBox::beforeDraw() {
	glDepthMask(GL_FALSE);
}

void SkyBox::afterDraw() {
	glDepthMask(GL_TRUE);
}

void SkyBox::setupVertices() {

	/************ Vertices ************/
	glm::vec3 p0(-25, -25, 25);
	glm::vec3 p1(25, -25, 25);
	glm::vec3 p2(25, -25, -25);
	glm::vec3 p3(-25, -25, -25);

	glm::vec3 p4(-25, 25, 25);
	glm::vec3 p5(25, 25, 25);
	glm::vec3 p6(25, 25, -25);
	glm::vec3 p7(-25, 25, -25);

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
}