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
	this->material->shader->setUniform("normalMatrix", variables.normalMatrix);
	this->material->shader->setUniform("viewMatrix", variables.viewMatrix);
	this->material->shader->setUniform("projectionMatrix", variables.projectionMatrix);
	this->material->shader->setUniform("cameraPosition", variables.cameraPosition);

	unsigned int lightIndex = 0;
	unsigned int spotIndex = 0;
	for (auto &light : variables.lights) {
		auto spot = std::dynamic_pointer_cast<SpotLight>(light.second);
		if (spot) {
			std::string lightIndexString = "spotLights[" + std::to_string(spotIndex) + "].";
			glm::vec3 spotPosition = glm::vec3(variables.viewMatrix * spot->getModelMatrix() * glm::vec4(0, 0, 0, 1));
			glm::vec3 spotDirection = glm::vec3(variables.viewMatrix * glm::vec4(spot->GetDirection(), 1)) - spotPosition;

			this->material->shader->setUniform(lightIndexString + "ambient", light.second->Ambient);
			this->material->shader->setUniform(lightIndexString + "diffuse", light.second->Diffuse);
			this->material->shader->setUniform(lightIndexString + "specular", light.second->Specular);
			this->material->shader->setUniform(lightIndexString + "intensity", light.second->GetIntensity());
			this->material->shader->setUniform(lightIndexString + "cutoff", spot->GetCutoff());
			this->material->shader->setUniform(lightIndexString + "innerCutoff", spot->GetInnerCutoff());
			this->material->shader->setUniform(lightIndexString + "exponent", spot->GetExponent());
			this->material->shader->setUniform(lightIndexString + "position", spotPosition);
			this->material->shader->setUniform(lightIndexString + "direction", spotDirection);
			spotIndex++;
		}
		else {
			std::string lightIndexString = "lights[" + std::to_string(lightIndex) + "].";
			glm::vec3 position = glm::vec3(variables.viewMatrix * light.second->getModelMatrix() * glm::vec4(0, 0, 0, 1));

			this->material->shader->setUniform(lightIndexString + "ambient", light.second->Ambient);
			this->material->shader->setUniform(lightIndexString + "diffuse", light.second->Diffuse);
			this->material->shader->setUniform(lightIndexString + "specular", light.second->Specular);
			this->material->shader->setUniform(lightIndexString + "intensity", light.second->GetIntensity());
			this->material->shader->setUniform(lightIndexString + "position", position);
			lightIndex++;
		}
	}
	this->material->shader->setUniform("lightNumber", lightIndex);
	this->material->shader->setUniform("spotNumber", spotIndex);

	for (auto &cu : this->_customUniforms)
		this->material->shader->setUniform(cu.first, cu.second);
}
