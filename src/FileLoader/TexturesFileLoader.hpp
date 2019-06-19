#ifndef SPIRITENGINE_TEXTURESFILELOADER_HPP
#define SPIRITENGINE_TEXTURESFILELOADER_HPP

#include <unordered_map>

#include "AFileLoader.hpp"
#include "Singleton.hpp"

#define ATTRIBUTE_CHECKER(n, t) \
  if (texture.HasMember(t)) { \
    assert(texture[t].IsString()); \
    tex->n = GetEnumFromString(texture[t].GetString(), n##Dict); \
  }

#define GET_SKYBOX_TEXTURE(name, face) \
  assert(texture["images"].HasMember(#name) && texture["images"][name].IsString()); \
  cubeMap[face] = CleanFilePath(texture["images"][name].GetString(), fileDirectory);

class TexturesFileLoader : public Singleton<TexturesFileLoader>, public AFileLoader {
	friend Singleton<TexturesFileLoader>;

 private:
	explicit TexturesFileLoader() = default;
	virtual ~TexturesFileLoader() = default;

 public:
	inline static void LoadFile(const std::string &filename) {
		TexturesFileLoader::getSingletonPtr()->LoadAndReadingFile(filename);
	}

 private:
	virtual void FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory);

	template<typename E>
	static E GetEnumFromString(const std::string &str, const std::unordered_map<std::string, E> &map) {
		assert(map.find(str) != map.end());
		return map.at(str);
	}
};

#endif /* !SPIRITENGINE_TEXTURESFILELOADER_HPP */
