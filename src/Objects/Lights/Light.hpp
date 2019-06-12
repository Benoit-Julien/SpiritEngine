#ifndef COMPUTERGRAPHICS1_LIGHT_HPP
#define COMPUTERGRAPHICS1_LIGHT_HPP

#include "Objects/Movable.hpp"

class Light : public Movable {
 private:
	float _radius;

 public:
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	float Intensity;

	static float Constant;
	static float Linear;
	static float Quadratic;

 public:
	explicit Light(const float &intensity = 1.0f);
	virtual ~Light();

	Light(const Light &light);
	Light &operator=(const Light &light);

	inline const float &GetRadius() const { return this->_radius; }

	void UpdateLightVolume();
};

#endif /* !COMPUTERGRAPHICS1_LIGHT_HPP */
