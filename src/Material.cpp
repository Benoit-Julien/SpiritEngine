#include <algorithm>

#include "Material.hpp"
#include "DefaultShader.hpp"

Material::Material() {
	this->shader = std::make_shared<ShaderProgram>();
	this->shader->initFromStrings(default_vertex_shader, default_fragment_shader);

	this->initialize();
}

Material::Material(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {
	this->shader = std::make_shared<ShaderProgram>();
	this->shader->initFromFiles(vertexShaderFilename, fragmentShaderFilename);

	this->initialize();
}

Material::~Material() {}

void Material::use() {
	this->shader->use();
	this->shader->setUniform("material.ambient", this->Ambient);
	this->shader->setUniform("material.diffuse", this->Diffuse);
	this->shader->setUniform("material.specular", this->Specular);
	this->shader->setUniform("material.shiness", this->shiness);

	if (!this->textures.empty()) {
		int index = 0;
		for (auto &text : this->textures) {
			text->use();
			this->shader->setUniform("material.tex" + std::to_string(index + 1), index);
			index++;
		}
	}
}

void Material::disable() {
	if (!this->textures.empty())
		for (auto &text : this->textures)
			text->disable();
	this->shader->disable();
}

void Material::AddTexture(std::shared_ptr<Texture> texture) {
	FIND_TEXTURE
	if (t != this->textures.end())
#ifdef _DEBUG
	{
		std::cerr << "Cannot add texture to this material because it's already present." << std::endl;
		return;
	}
#elif
		return;
#endif
	this->textures.push_back(texture);
}

void Material::RemoveTexture(std::shared_ptr<Texture> texture) {
	FIND_TEXTURE
	if (t == this->textures.end())
#ifdef _DEBUG
	{
		std::cerr << "Cannot remove this texture to this material because it's not present." << std::endl;
		return;
	}
#elif
	return;
#endif
	this->textures.erase(t);
}

void Material::initialize() {
	this->Diffuse = glm::vec3(1, 1, 1);
	this->Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	this->Specular = glm::vec3(1, 1, 1);
	this->shiness = 35.0f;
}