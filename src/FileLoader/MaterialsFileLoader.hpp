#ifndef SPIRITENGINE_MATERIALSFILELOADER_HPP
#define SPIRITENGINE_MATERIALSFILELOADER_HPP

#include <glm/vec3.hpp>

#include "AFileLoader.hpp"
#include "Singleton.hpp"

class MaterialsFileLoader : public Singleton<MaterialsFileLoader>, public AFileLoader {
	friend Singleton<MaterialsFileLoader>;

 private:
	explicit MaterialsFileLoader() = default;
	virtual ~MaterialsFileLoader() = default;

 public:
	inline static void LoadFile(const std::string &filename) {
		MaterialsFileLoader::getSingletonPtr()->LoadAndReadingFile(filename);
	}

 private:
	virtual void FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory);

	glm::vec3 getColor(const rapidjson::Value &value);
};

#endif /* !SPIRITENGINE_MATERIALSFILELOADER_HPP */
