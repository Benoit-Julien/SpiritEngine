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
}

void Material::disable() {
	this->shader->disable();
}

void Material::initialize() {
	this->Diffuse = glm::vec3(1, 1, 1);
	this->Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	this->Specular = glm::vec3(1, 1, 1);
	this->shiness = 35.0f;
}