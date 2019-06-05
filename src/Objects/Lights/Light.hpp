#ifndef COMPUTERGRAPHICS1_LIGHT_HPP
#define COMPUTERGRAPHICS1_LIGHT_HPP

#include "Objects/Movable.hpp"

class Light : public Movable {
 private:
	float _intensity;
	float _radius;

 public:
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;

	static float Constant;
	static float Linear;
	static float Quadratic;

 public:
	Light(const float &intensity = 1.0f);
	virtual ~Light();

	Light(const Light &light);
	Light &operator=(const Light &light);

	inline void SetIntensity(const float &intensity) { this->_intensity = intensity; }

	inline const float &GetIntensity() const { return this->_intensity; }

	inline const float &GetRadius() const { return this->_radius; }

	void UpdateLightVolume();
};

#endif /* !COMPUTERGRAPHICS1_LIGHT_HPP */
