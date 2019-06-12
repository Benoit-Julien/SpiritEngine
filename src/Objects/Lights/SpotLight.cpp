#include "SpotLight.hpp"

SpotLight::SpotLight(const float &intensity)
				: Light(intensity),
					Cutoff(30.0f),
					InnerCutoff(20.0f),
					Exponent(10.0f) {}

SpotLight::~SpotLight() {}

SpotLight::SpotLight(const SpotLight &spot)
				: Light(spot),
					Cutoff(spot.Cutoff),
					InnerCutoff(spot.InnerCutoff),
					Exponent(spot.Exponent) {}

SpotLight &SpotLight::operator=(const SpotLight &spot) {
	this->Cutoff = spot.Cutoff;
	this->InnerCutoff = spot.InnerCutoff;
	this->Exponent = spot.Exponent;

	return *this;
}