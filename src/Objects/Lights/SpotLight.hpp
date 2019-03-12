#ifndef SPIRITENGINE_SPOTLIGHT_HPP
#define SPIRITENGINE_SPOTLIGHT_HPP

#include "Light.hpp"

class SpotLight : public Light {
 private:
	float _cutoff;
	float _innerCutoff;
	float _exponent;
	glm::vec3 _direction;

 public:
	SpotLight(const glm::vec3 &direction, const float &intensity = 1.0f);
	virtual ~SpotLight();

	SpotLight(const SpotLight &spot);
	SpotLight &operator=(const SpotLight &spot);

	inline void SetCutoff(const float &cutoff) { this->_cutoff = cutoff; }
	inline void SetInnerCutoff(const float &innerCutoff) { this->_innerCutoff = innerCutoff; }
	inline void SetExponent(const float &exponent) { this->_exponent = exponent; }
	inline void SetDirection(const glm::vec3 &direction) { this->_direction = direction; }

	inline const float &GetCutoff() const { return this->_cutoff; }
	inline const float &GetInnerCutoff() const { return this->_innerCutoff; }
	inline const float &GetExponent() const { return this->_exponent; }
	inline const glm::vec3 &GetDirection() const { return this->_direction; }
};


#endif /* !SPIRITENGINE_SPOTLIGHT_HPP */
