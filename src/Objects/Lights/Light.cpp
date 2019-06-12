#include "Light.hpp"

float Light::Constant = 1.0f;
float Light::Linear = 0.1f;
float Light::Quadratic = 0.01f;

Light::Light(const float &intensity) : Movable(ObjectType::Light) {
	this->Intensity = intensity;

	this->Diffuse = glm::vec3(1, 1, 1);
	this->Ambient = glm::vec3(1, 1, 1);
	this->Specular = glm::vec3(1, 1, 1);
}

Light::~Light() {}

Light::Light(const Light &light)
				: Movable(light),
					Intensity(light.Intensity),
					Ambient(light.Ambient),
					Diffuse(light.Diffuse),
					Specular(light.Specular) {

}

Light &Light::operator=(const Light &light) {
	this->Intensity = light.Intensity;
	this->Ambient = light.Ambient;
	this->Diffuse = light.Diffuse;
	this->Specular = light.Specular;
	return *this;
}

void Light::UpdateLightVolume() {
	float lightMax = std::fmaxf(std::fmaxf(this->Diffuse.r, this->Diffuse.g), this->Diffuse.b);
	this->_radius = (-Light::Linear + std::sqrtf(Light::Linear * Light::Linear - 4 * Light::Quadratic * Light::Constant - (256.0 / 5.0) * lightMax))
					/ (2 * Light::Quadratic);
	this->_radius *= this->Intensity;
}