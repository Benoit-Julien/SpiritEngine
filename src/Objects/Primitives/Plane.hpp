#ifndef COMPUTERGRAPHICS1_PLANE_HPP
#define COMPUTERGRAPHICS1_PLANE_HPP

#include "../TriangleObject.hpp"

class Plane : public TriangleObject {
 public:
	Plane();
	virtual ~Plane();

	Plane(const Plane &plane);
	Plane &operator=(const Plane &plane);

 private:
	void setupVertices();
};

#endif /* !COMPUTERGRAPHICS1_PLANE_HPP */
