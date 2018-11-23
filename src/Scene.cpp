#include <exception>
#include <iostream>
#include <algorithm>
#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include "Scene.hpp"
#include "Objects/ModelLoader.hpp"

template<>
std::shared_ptr<Model3D> Scene::CreateObject<Model3D, std::string>(const std::string &fileName) {
	auto self = Scene::getSingletonPtr();

	auto model = ModelLoader::LoadModel(fileName);
	self->_objects[model->getObjectID()] = model;
	return model;
}

void Scene::RemoveObject(const unsigned int &ID) {
	auto self = Scene::getSingletonPtr();

	auto elem = self->_objects.find(ID);
	if (elem == self->_objects.end())
		throw std::logic_error("Cannot remove object where id is " + std::to_string(ID) + " because it's not exist.");
	self->_objects.erase(elem);
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

static glm::vec3 getColor(const rapidjson::Value &value) {
	assert(value.IsArray());
	assert(value.GetArray().Size() == 3);
	assert(value.GetArray()[0].IsFloat());
	assert(value.GetArray()[1].IsFloat());
	assert(value.GetArray()[2].IsFloat());

	float r = value.GetArray()[0].GetFloat();
	float g = value.GetArray()[1].GetFloat();
	float b = value.GetArray()[2].GetFloat();
	return glm::vec3(r, g, b);
}

void Scene::LoadMaterialFile(const std::string &fileName) {
	if (fileName.rfind(".json") == std::string::npos)
		throw std::logic_error("Cannot load materials file (" + fileName + ") because it isn't a json file.");

	std::ifstream ifs(fileName);
	if (!ifs)
		throw std::logic_error("Cannot read materials file : " + fileName);

	std::string fileDirectory;
	std::size_t slashPos = fileName.rfind('/');
	if (slashPos != std::string::npos)
		fileDirectory = fileName.substr(0, slashPos) + "/";

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	if (document.HasParseError()) {
		throw std::runtime_error("Error(offset "
														 + std::to_string(document.GetErrorOffset())
														 + ": " + GetParseError_En(document.GetParseError()));
	}

	assert(document.IsObject());
	assert(document.HasMember("materials") && document["materials"].IsArray());
	for (auto &material : document["materials"].GetArray()) {
		assert(material.IsObject());
		assert(material.HasMember("name") && material["name"].IsString());
		assert(material.HasMember("vertex") && material["vertex"].IsString());
		assert(material.HasMember("fragment") && material["fragment"].IsString());

		std::string vertexShader = material["vertex"].GetString();
		std::string fragmentShader = material["fragment"].GetString();
		if (vertexShader[0] != '/')
			vertexShader = fileDirectory + vertexShader;
		if (fragmentShader[0] != '/')
			fragmentShader = fileDirectory + fragmentShader;

		auto mat = Scene::CreateMaterial(material["name"].GetString(), vertexShader, fragmentShader);

		if (material.HasMember("diffuse"))
			mat->Diffuse = getColor(material["diffuse"]);
		if (material.HasMember("ambient"))
			mat->Ambient = getColor(material["ambient"]);
		if (material.HasMember("specular"))
			mat->Specular = getColor(material["specular"]);
		if (material.HasMember("phong")) {
			assert(material["phong"].IsFloat());
			mat->phong = material["phong"].GetFloat();
		}
	}
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

	obj->material->use();
	{
		obj->material->shader->setUniform("modelMatrix", m);
		obj->material->shader->setUniform("normalMatrix", n);
		obj->material->shader->setUniform("viewMatrix", info.viewMatrix);
		obj->material->shader->setUniform("projectionMatrix", info.projectionMatrix);

		obj->material->shader->setUniform("lightNumber", lightNumber);
		this->setLightsToShader(obj->material, info);

		obj->Draw();
	}
	obj->material->disable();

	for (auto &children : obj->Children) {
		Scene::recursiveDraw(std::dynamic_pointer_cast<Drawable>(children), info, m);
	}
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