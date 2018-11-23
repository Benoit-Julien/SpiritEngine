#ifndef COMPUTERGRAPHICS1_SPHERE_HPP
#define COMPUTERGRAPHICS1_SPHERE_HPP

#include "../TriangleObject.hpp"

class Sphere : public TriangleObject {
	float _radius;
	int _nbLong;
	int _nbLat;

 public:
	Sphere(const float &radius = 1, const int &nbLong = 24, const int &nbLat = 16);
	virtual ~Sphere();

	Sphere(const Sphere &sphere);
	Sphere &operator=(const Sphere &sphere);

 private:
	void setupVertices();
};


#endif /* !COMPUTERGRAPHICS1_SPHERE_HPP */
