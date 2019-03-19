#ifndef SPIRITENGINE_TEXTURESFILELOADER_HPP
#define SPIRITENGINE_TEXTURESFILELOADER_HPP

#include <unordered_map>

#include "AFileLoader.hpp"
#include "Singleton.hpp"

#define ATTRIBUTE_CHECKER(n, t) \
	if (texture.HasMember(t)) { \
		assert(texture[t].IsString()); \
		text->n = GetEnumFromString(texture[t].GetString(), n##Dict); \
	}

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
