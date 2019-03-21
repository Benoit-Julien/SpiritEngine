#include <exception>
#include <iostream>
#include <algorithm>

#include "Scene.hpp"
#include "Objects/ModelLoader.hpp"
#include "Global.hpp"

std::shared_ptr<Mesh> Scene::CreateMesh(const std::string &name) {
	auto self = Scene::getSingletonPtr();

	auto model = ModelLoader::LoadModel(name);
	self->_objects[model->getObjectID()] = model;
	return model;
}

std::shared_ptr<Drawable> Scene::FindObjectByID(const unsigned int &ID) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_objects.find(ID);
	if (elem == self->_objects.end()) {
		std::cerr << "Cannot find object where id is " << ID << " because it's not exist." << std::endl;
		return nullptr;
	}
	return elem->second;
}

std::shared_ptr<Drawable> Scene::FindObjectByType(const ObjectType &type) {
	auto self = Scene::getSingletonPtr();

	for (auto &obj : self->_objects) {
		if (obj.second->getType() == type)
			return obj.second;
	}
	std::cerr << "Cannot find object of this type because it's not exist." << std::endl;
	return nullptr;
}

std::vector<std::shared_ptr<Drawable>> Scene::FindObjectsByType(const ObjectType &type) {
	auto self = Scene::getSingletonPtr();

	std::vector<std::shared_ptr<Drawable>> list;
	for (auto &obj : self->_objects) {
		if (obj.second->getType() == type)
			list.push_back(obj.second);
	}

	if (list.empty())
		std::cerr << "Cannot find object of this type because it's not exist." << std::endl;
	return list;
}

void Scene::Destroy(std::shared_ptr<Drawable> object, const float &time) {
	auto self = Scene::getSingletonPtr();

	auto timer = std::chrono::milliseconds((long) (time * 1000));
	self->_toDestroy.push_back(std::make_pair(object, std::chrono::system_clock::now() + timer));
}

void Scene::RemoveMaterial(const std::string &name) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_materials.find(name);
	if (elem == self->_materials.end())
		throw std::logic_error("Cannot remove material named " + name + " because it's not exist.");
	self->_materials.erase(elem);
}

std::shared_ptr<Material> Scene::FindMaterial(const std::string &name) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_materials.find(name);
	if (elem == self->_materials.end()) {
		std::cerr << "Cannot find material named " << name << " because it's not exist." << std::endl;
		return nullptr;
	}
	return elem->second;
}

void Scene::RemoveTexture(const std::string &name) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_textures.find(name);
	if (elem == self->_textures.end())
		throw std::logic_error("Cannot remove texture named " + name + " because it's not exist.");
	self->_textures.erase(elem);
}

std::shared_ptr<Texture> Scene::FindTexture(const std::string &name) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_textures.find(name);
	if (elem == self->_textures.end()) {
		std::cerr << "Cannot find texture named " << name << " because it's not exist." << std::endl;
		return nullptr;
	}
	return elem->second;
}

void Scene::RemoveLight(const unsigned int &ID) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_lights.find(ID);
	if (elem == self->_lights.end())
		throw std::logic_error("Cannot remove light where id is " + std::to_string(ID) + " because it's not exist.");
	self->_lights.erase(elem);
}

std::shared_ptr<Light> Scene::FindLight(const unsigned int &ID) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_lights.find(ID);
	if (elem == self->_lights.end()) {
		std::cerr << "Cannot find light where id is " << ID << " because it's not exist." << std::endl;
		return nullptr;
	}
	return elem->second;
}

void Scene::BeforeDrawing() {
	auto self = Scene::getSingletonPtr();
	auto now = std::chrono::system_clock::now();
	auto it = self->_toDestroy.begin();

	while (it != self->_toDestroy.end()) {
		if (it->second < now) {
			self->_objects.erase(it->first->getObjectID());
			it = self->_toDestroy.erase(it);
		}
		else
			it++;
	}
}

void Scene::PhysicalUpdate() {

}

void Scene::Draw(const DrawInformation &info) {
	auto self = Scene::getSingletonPtr();

	for (auto &obj : self->_objects) {
		if (obj.second->Parent == nullptr)
			self->recursiveDraw(obj.second, info, glm::mat4(1.0f));
	}
}

void Scene::recursiveDraw(std::shared_ptr<Drawable> obj, const DrawInformation &info, const glm::mat4 &model) {
	if (!obj)
		return;

	glm::mat4 m = model * obj->getModelMatrix();
	glm::mat3 n = glm::mat3(glm::transpose(glm::inverse(info.viewMatrix * m)));
	GLuint lightNumber = this->_lights.size();

	obj->Draw(ShaderVariables(m, n, info.viewMatrix, info.projectionMatrix, this->_lights));

	for (auto &children : obj->Children)
		Scene::recursiveDraw(std::dynamic_pointer_cast<Drawable>(children), info, m);
}

void Scene::setLightsToShader(std::shared_ptr<Material> material, const DrawInformation &info) {
	unsigned int index = 0;
	for (auto &light : this->_lights) {
		std::string lightindex = "lights[" + std::to_string(index) + "].";
		glm::vec3 position = glm::vec3(info.viewMatrix * light.second->getModelMatrix() * glm::vec4(0, 0, 0, 1));

		material->shader->setUniform(lightindex + "ambient", light.second->Ambient);
		material->shader->setUniform(lightindex + "diffuse", light.second->Diffuse);
		material->shader->setUniform(lightindex + "specular", light.second->Specular);
		material->shader->setUniform(lightindex + "intensity", light.second->GetIntensity());
		material->shader->setUniform(lightindex + "position", position);

		index++;
	}
}