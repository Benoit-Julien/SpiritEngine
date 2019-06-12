#ifndef SPIRITENGINE_DIRECTIONALLIGHT_HPP
#define SPIRITENGINE_DIRECTIONALLIGHT_HPP

#include "Light.hpp"

class DirectionalLight : public Light {
 public:
	explicit DirectionalLight(const float &intensity = 1.0f) : Light(intensity) { };
	virtual ~DirectionalLight() = default;
};

#endif /* !SPIRITENGINE_DIRECTIONALLIGHT_HPP */
