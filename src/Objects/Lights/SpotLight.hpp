#ifndef SPIRITENGINE_SPOTLIGHT_HPP
#define SPIRITENGINE_SPOTLIGHT_HPP

#include "Light.hpp"

class SpotLight : public Light {
 public:
	float Cutoff;
	float InnerCutoff;
	float Exponent;

 public:
	explicit SpotLight(const float &intensity = 1.0f);
	virtual ~SpotLight();

	SpotLight(const SpotLight &spot);
	SpotLight &operator=(const SpotLight &spot);
};


#endif /* !SPIRITENGINE_SPOTLIGHT_HPP */
