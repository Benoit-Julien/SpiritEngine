#ifndef SPIRITENGINE_SHADERSFILELOADER_HPP
#define SPIRITENGINE_SHADERSFILELOADER_HPP

#include "AFileLoader.hpp"
#include "Singleton.hpp"

class ShaderFileLoader : public Singleton<ShaderFileLoader>, public AFileLoader {
	friend Singleton<ShaderFileLoader>;

 private:
	ShaderFileLoader() = default;
	virtual ~ShaderFileLoader() = default;

 public:
	inline static void LoadFile(const std::string &filename) {
		ShaderFileLoader::getSingletonPtr()->LoadAndReadingFile(filename);
	}

 private:
	virtual void FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory);
};

#endif /* !SPIRITENGINE_SHADERSFILELOADER_HPP */
