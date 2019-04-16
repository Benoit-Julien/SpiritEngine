#include <algorithm>

#include "Material.hpp"
#include "DefaultShader.hpp"
#include "Scene.hpp"

Material::Material() {
	this->shader = std::make_shared<ShaderProgram>();
	this->shader->initFromStrings(default_vertex_shader, default_fragment_shader);

	this->initialize();
}

Material::Material(const std::string &vertexShader, const std::string &fragmentShader) {
	this->shader = std::make_shared<ShaderProgram>();
	auto vertex = Scene::FindShader<VertexShader>(vertexShader);
	auto fragment = Scene::FindShader<FragmentShader>(fragmentShader);
	this->shader->initFromShader(vertex, fragment);

	this->initialize();
}

Material::Material(std::shared_ptr<ShaderProgram> shader) {
	this->shader = shader;
	this->initialize();
}

Material::~Material() {}

void Material::use() {
	this->shader->use();
	this->shader->setUniform("material.ambient", this->Ambient);
	this->shader->setUniform("material.diffuse", this->Diffuse);
	this->shader->setUniform("material.specular", this->Specular);
	this->shader->setUniform("material.shiness", this->shiness);

	int index = 0;
	if (!this->textures.empty()) {
		for (auto &text : this->textures) {
			glActiveTexture(GL_TEXTURE0 + index);
			text->use();
			this->shader->setUniform("material.textures[" + std::to_string(index) + "]", index);
			index++;
		}
		this->shader->setUniform("material.textNumber", (unsigned int) this->textures.size());
	}
	if (this->normalMap != nullptr) {
		glActiveTexture(GL_TEXTURE0 + index);
		this->normalMap->use();
		this->shader->setUniform("material.normalTex", index);
		index++;
	}
}

void Material::disable() {
	if (!this->textures.empty()) {
		for (auto &text : this->textures)
			text->disable();
	}
	if (this->normalMap)
		this->normalMap->disable();
	this->shader->disable();
}

void Material::AddTexture(std::shared_ptr<Texture> texture) {
	if (this->textures.size() == MAX_TEXTURES)
		return;

	FIND_TEXTURE
	if (t != this->textures.end())
#ifdef NDEBUG
		{
			std::cerr << "Cannot add texture to this material because it's already present." << std::endl;
			return;
		}
#else
		return;
#endif
	this->textures.push_back(texture);
}

void Material::RemoveTexture(std::shared_ptr<Texture> texture) {
	FIND_TEXTURE
	if (t == this->textures.end())
#ifdef NDEBUG
		{
			std::cerr << "Cannot remove this texture to this material because it's not present." << std::endl;
			return;
		}
#else
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