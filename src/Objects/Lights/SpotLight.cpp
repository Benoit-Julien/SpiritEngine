#include "SpotLight.hpp"

SpotLight::SpotLight(const glm::vec3 &direction, const float &intensity)
				: Light(intensity),
					_cutoff(30.0f),
					_innerCutoff(20.0f),
					_exponent(10.0f),
					_direction(direction) {}

SpotLight::~SpotLight() {}

SpotLight::SpotLight(const SpotLight &spot)
				: Light(spot),
					_cutoff(spot._cutoff),
					_innerCutoff(spot._innerCutoff),
					_exponent(spot._exponent),
					_direction(spot._direction) {}

SpotLight &SpotLight::operator=(const SpotLight &spot) {
	this->_cutoff = spot._cutoff;
	this->_innerCutoff = spot._innerCutoff;
	this->_exponent = spot._exponent;
	this->_direction = spot._direction;

	return *this;
}