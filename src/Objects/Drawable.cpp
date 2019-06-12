#include "Drawable.hpp"
#include "Objects/Lights/Light.hpp"
#include "Objects/Lights/SpotLight.hpp"

Drawable::Drawable(const ObjectType &type) : Movable(type) {
	this->_enabled = true;
}

Drawable::~Drawable() {}

Drawable::Drawable(const Drawable &drawable)
				: Movable(drawable),
					_enabled(drawable._enabled),
					material(drawable.material) {}

Drawable &Drawable::operator=(const Drawable &drawable) {
	this->_enabled = drawable._enabled;
	this->material = drawable.material;
	return *this;
}

void Drawable::RemoveCustomUniform(const std::string &varName) {
	auto cu = this->_customUniforms.find(varName);
	if (cu != this->_customUniforms.end())
		this->_customUniforms.erase(cu);
}

void Drawable::updateShaderUniform(const ShaderVariables &variables) {
	this->material->shader->setUniform("modelMatrix", variables.modelMatrix);
	this->material->shader->setUniform("viewMatrix", variables.viewMatrix);
	this->material->shader->setUniform("projectionMatrix", variables.projectionMatrix);
	this->material->shader->setUniform("normalMatrix", variables.normalMatrix);

	for (auto &cu : this->_customUniforms)
		this->material->shader->setUniform(cu.first, cu.second);
}
