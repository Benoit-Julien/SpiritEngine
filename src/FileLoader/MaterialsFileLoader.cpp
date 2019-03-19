#include "MaterialsFileLoader.hpp"
#include "Scene.hpp"

void MaterialsFileLoader::FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory) {
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
		if (material.HasMember("shiness")) {
			assert(material["shiness"].IsNumber());
			mat->shiness = material["shiness"].GetFloat();
		}
		if (material.HasMember("texture")) {
			assert(!material["texture"].IsString() || !material["texture"].IsArray());
			if (material["texture"].IsString())
				mat->AddTexture(Scene::FindTexture(material["texture"].GetString()));
			else if (material["texture"].IsArray()) {
				for (auto &texture : material["texture"].GetArray()) {
					assert(texture.IsString());
					mat->AddTexture(Scene::FindTexture(texture.GetString()));
				}
			}
		}
	}
}

glm::vec3 MaterialsFileLoader::getColor(const rapidjson::Value &value) {
	assert(value.IsArray());
	assert(value.GetArray().Size() >= 3);
	assert(value.GetArray()[0].IsNumber());
	assert(value.GetArray()[1].IsNumber());
	assert(value.GetArray()[2].IsNumber());

	float r = value.GetArray()[0].GetFloat();
	float g = value.GetArray()[1].GetFloat();
	float b = value.GetArray()[2].GetFloat();
	return glm::vec3(r, g, b);
}