#ifndef COMPUTERGRAPHICS1_CHECKEREDFLOOR_HPP
#define COMPUTERGRAPHICS1_CHECKEREDFLOOR_HPP

#include "../TriangleObject.hpp"

class CheckeredFloor : public TriangleObject {
	const int _size;
	const int _nsqure;

 public:
	CheckeredFloor(const int &size, const int &nsqure);
	virtual ~CheckeredFloor();

 private:
	void setupVertices();
};

#endif /* !COMPUTERGRAPHICS1_CHECKEREDFLOOR_HPP */
