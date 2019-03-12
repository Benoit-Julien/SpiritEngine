#include "Light.hpp"

Light::Light(const float &intensity) : Movable(ObjectType::Light) {
	this->_intensity = intensity;

	this->Diffuse = glm::vec3(1, 1, 1);
	this->Ambient = glm::vec3(1, 1, 1);
	this->Specular = glm::vec3(1, 1, 1);
}

Light::~Light() {}

Light::Light(const Light &light)
				: Movable(light),
					_intensity(light._intensity),
					Ambient(light.Ambient),
					Diffuse(light.Diffuse),
					Specular(light.Specular) {

}

Light &Light::operator=(const Light &light) {
	this->_intensity = light._intensity;
	this->Ambient = light.Ambient;
	this->Diffuse = light.Diffuse;
	this->Specular = light.Specular;
	return *this;
}