#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "Sphere.hpp"

Sphere::Sphere(const float &radius, const int &nbLong, const int &nbLat)
				: TriangleObject(ObjectType::Sphere),
					_radius(radius),
					_nbLong(nbLong),
					_nbLat(nbLat) {
	this->setupVertices();
	this->setup();
}

Sphere::~Sphere() {}

Sphere::Sphere(const Sphere &sphere)
				: TriangleObject(sphere),
					_radius(sphere._radius),
					_nbLong(sphere._nbLong),
					_nbLat(sphere._nbLat) {

}

Sphere &Sphere::operator=(const Sphere &sphere) {
	this->_radius = sphere._radius;
	this->_nbLong = sphere._nbLong;
	this->_nbLat = sphere._nbLat;
	return *this;
}

void Sphere::setupVertices() {
	float _2pi = glm::pi<double>() * 2;

	/************ Vertices ************/
	this->vertices = std::vector<glm::vec3>((this->_nbLong + 1) * this->_nbLat + 2);

	this->vertices[0] = glm::vec3(0, 1, 0) * this->_radius;
	for (int lat = 0; lat < this->_nbLat; lat++) {
		float a1 = glm::pi<double>() * (float) (lat + 1) / (this->_nbLat + 1);
		float sin1 = glm::sin(a1);
		float cos1 = glm::cos(a1);

		for (int lon = 0; lon <= this->_nbLong; lon++) {
			float a2 = _2pi * (float) (lon == this->_nbLong ? 0 : lon) / this->_nbLong;
			float sin2 = glm::sin(a2);
			float cos2 = glm::cos(a2);

			int index = lon + lat * (this->_nbLong + 1) + 1;
			this->vertices[index] = glm::vec3(sin1 * cos2, cos1, sin1 * sin2) * this->_radius;
		}
	}
	this->vertices[this->vertices.size() - 1] = glm::vec3(0, 1, 0) * -this->_radius;

	/************ Normals ************/
	this->normals = std::vector<glm::vec3>(this->vertices.size());
	for (int n = 0; n < this->vertices.size(); n++)
		this->normals[n] = glm::normalize(this->vertices[n]);

	/************ Triangles ************/
	int nbFaces = this->vertices.size();
	int nbTriangles = nbFaces * 2;
	int nbIndexes = nbTriangles * 3;
	this->triangles = std::vector<TriangleIndex>(nbIndexes);

	int i = 0;
	for (int lon = 0; lon < this->_nbLong; lon++)
		this->triangles[i++] = TriangleIndex(lon + 2, lon + 1, 0);

//Middle
	for (int lat = 0; lat < this->_nbLat - 1; lat++) {
		for (int lon = 0; lon < this->_nbLong; lon++) {
			int current = lon + lat * (this->_nbLong + 1) + 1;
			int next = current + this->_nbLong + 1;

			this->triangles[i++] = TriangleIndex(current, current + 1, next + 1);

			this->triangles[i++] = TriangleIndex(current, next + 1, next);
		}
	}

//Bottom Cap
	for (int lon = 0; lon < this->_nbLong; lon++) {
		this->triangles[i++] = TriangleIndex(this->vertices.size() - 1,
																				 this->vertices.size() - (lon + 2) - 1,
																				 this->vertices.size() - (lon + 1) - 1);
	}
}