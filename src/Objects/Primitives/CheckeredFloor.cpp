#include "CheckeredFloor.hpp"

CheckeredFloor::CheckeredFloor(const int &size, const int &nsqure)
				: TriangleObject(ObjectType::CheckeredFloor),
					_size(size),
					_nsqure(nsqure) {
	this->setupVertices();
	this->setup();
}

CheckeredFloor::~CheckeredFloor() {

}

void CheckeredFloor::setupVertices() {
	glm::vec4 floorColor1(.7f, .7f, .7f, 1); // Light color
	glm::vec4 floorColor2(.3f, .3f, .3f, 1); // Dark color

	float maxX = this->_size / 2, maxY = this->_size / 2;
	float minX = -this->_size / 2, minY = -this->_size / 2;
	int x, y, v[3], i;
	float xp, yp, xd, yd;
	v[2] = 0;
	xd = (maxX - minX) / ((float) this->_nsqure);
	yd = (maxY - minY) / ((float) this->_nsqure);
	GLushort currentIndex = 0;

	for (x = 0, xp = minX; x < this->_nsqure; x++, xp += xd) {
		for (y = 0, yp = minY, i = x; y < this->_nsqure; y++, i++, yp += yd) {
			glm::vec4 color = i % 2 == 1 ? floorColor1 : floorColor2;

			this->vertices.push_back(glm::vec3(xp, 0, yp));
			this->colors.push_back(color);
			this->vertices.push_back(glm::vec3(xp, 0, yp + yd));
			this->colors.push_back(color);
			this->vertices.push_back(glm::vec3(xp + xd, 0, yp + yd));
			this->colors.push_back(color);
			this->vertices.push_back(glm::vec3(xp + xd, 0, yp));
			this->colors.push_back(color);

			this->triangles.push_back(TriangleIndex(currentIndex, currentIndex + 1, currentIndex + 2));
			this->triangles.push_back(TriangleIndex(currentIndex, currentIndex + 2, currentIndex + 3));
			currentIndex += 4;
		} // end of for j
	}// end of for i
}