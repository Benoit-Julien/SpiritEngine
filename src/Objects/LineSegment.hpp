#ifndef COMPUTERGRAPHICS1_LINESEGMENT_HPP
#define COMPUTERGRAPHICS1_LINESEGMENT_HPP

#include <vector>

#include "Object.hpp"

class LineSegment : public Object {
 private:
	GLuint _vao; /// id or handler
	GLuint _vboVertices;
	GLuint _vboColors;

 public:
	LineSegment();
	virtual ~LineSegment();

	virtual void draw();

 private:
	virtual void setup();
};


#endif /* !COMPUTERGRAPHICS1_LINESEGMENT_HPP */
