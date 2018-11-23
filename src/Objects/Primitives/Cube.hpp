#ifndef COMPUTERGRAPHICS1_COLORCUBE_HPP
#define COMPUTERGRAPHICS1_COLORCUBE_HPP

#include "../TriangleObject.hpp"

class Cube : public TriangleObject {
 public:
	Cube();
	virtual ~Cube();

	Cube(const Cube &cube);
	Cube &operator=(const Cube &cube);

 private:
	void setupVertices();
};

#endif /* !COMPUTERGRAPHICS1_COLORCUBE_HPP */
