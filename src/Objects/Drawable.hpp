#ifndef COMPUTERGRAPHICS1_DRAWABLE_HPP
#define COMPUTERGRAPHICS1_DRAWABLE_HPP

#include <unordered_map>

#include "Movable.hpp"
#include "Material.hpp"

class Drawable : public Movable {
	bool _enabled;
	bool _enabledCulling;
	GLenum _cullface;

	std::unordered_map<std::string, Shader::var_t> _customUniforms;

 public:
	std::shared_ptr<Material> material;

 public:
	Drawable(const ObjectType &type);
	virtual ~Drawable();

	Drawable(const Drawable &drawable);
	Drawable &operator=(const Drawable &drawable);

	template<typename T>
	inline void SetCustomUniform(const std::string &varName, const T &value) {
		this->_customUniforms[varName] = Shader::var_t(value);
	}

	void RemoveCustomUniform(const std::string &varName);

	virtual void Draw(const ShaderVariables &variables) = 0;

	inline void SetEnable(const bool &enable) { this->_enabled = enable; }
	inline const bool &Enabled() const { return this->_enabled; }

	inline void SetCullFaceOption(const GLenum &cullface) { this->_cullface = cullface; }
	inline const GLenum &GetCullFaceOption() const { return this->_cullface; }

	inline void EnableCulling() { this->_enabledCulling = true; }
	inline void DisableCulling() { this->_enabledCulling = false; }
	inline const bool &Culling() const { return this->_enabledCulling; }

 protected:
	void updateShaderUniform(const ShaderVariables &variables);
};

#endif /* !COMPUTERGRAPHICS1_DRAWABLE_HPP */
