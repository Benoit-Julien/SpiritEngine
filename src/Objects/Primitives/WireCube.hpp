#ifndef COMPUTERGRAPHICS1_WIRECUBE_HPP
#define COMPUTERGRAPHICS1_WIRECUBE_HPP

#include "../TriangleObject.hpp"

class WireCube : public TriangleObject {
 public:
	WireCube();
	virtual ~WireCube();

 private:
	void setupVertices();
};


#endif /* !COMPUTERGRAPHICS1_WIRECUBE_HPP */
