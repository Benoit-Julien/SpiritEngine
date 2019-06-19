#ifndef SPIRITENGINE_TEXTUREMANAGER_HPP
#define SPIRITENGINE_TEXTUREMANAGER_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

class TextureManager {
 public:
	explicit TextureManager();
	~TextureManager();

	inline const GLuint &Get(const std::string &texture) { return this->_textures.at(texture); }

	void Add(const std::string &texture);
	void CreateTexture(const std::string &texture, const unsigned &w, const unsigned &h, const GLuint &filter, const GLuint &internalFormat, const GLuint &format, const GLuint &type, const bool &depth);
	void CreateRGBA16F3DTexture(const std::string &texture, const glm::vec3 &dim, const GLuint &filter, const GLuint &wrap);
	void CreateRGBA3DTexture(const std::string &texture, const glm::vec3 &dim, const GLuint &filter, const GLuint &wrap);
	void Clear3Dtexture(const GLuint &texture);

	inline void SetClearTextureExtension(const bool &v) {
		this->_clearTextureExtension = v;
	}

 private:
	std::unordered_map<std::string, GLuint> _textures;
	bool _clearTextureExtension;
};

#endif