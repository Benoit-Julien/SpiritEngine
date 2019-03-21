#ifndef COMPUTERGRAPHICS1_DRAWABLE_HPP
#define COMPUTERGRAPHICS1_DRAWABLE_HPP

#include <unordered_map>

#include "Movable.hpp"
#include "CullFace.hpp"
#include "Material.hpp"

class Drawable : public Movable, public CullFace {
	bool _enabled;
	std::unordered_map<std::string, ShaderProgram::var_t> _customUniforms;

 public:
	std::shared_ptr<Material> material;

 public:
	Drawable(const ObjectType &type);
	virtual ~Drawable();

	Drawable(const Drawable &drawable);
	Drawable &operator=(const Drawable &drawable);

	template<typename T>
	inline void SetCustomUniform(const std::string &varName, const T &value) {
		this->_customUniforms[varName] = ShaderProgram::var_t(value);
	}

	void RemoveCustomUniform(const std::string &varName);

	virtual void Draw(const ShaderVariables &variables) = 0;

	inline void SetEnable(const bool &enable) { this->_enabled = enable; }
	inline const bool &Enabled() const { return this->_enabled; }

 protected:
	void updateShaderUniform(const ShaderVariables &variables);
};

#endif /* !COMPUTERGRAPHICS1_DRAWABLE_HPP */
