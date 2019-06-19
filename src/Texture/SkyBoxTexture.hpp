#ifndef SPIRITENGINE_SKYBOXTEXTURE_HPP
#define SPIRITENGINE_SKYBOXTEXTURE_HPP

#include <unordered_map>
#include "Texture.hpp"

class SkyBoxTexture : public Texture {
 public:
	enum class Face : GLuint {
		LEFT = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		RIGHT = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		TOP = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		DOWN = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		BACK = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		FRONT = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

 private:
	std::unordered_map<Face, std::string> _cubeMap;
	bool _initialised;

 public:
	explicit SkyBoxTexture();
	virtual ~SkyBoxTexture();

	void initFromCubeMap(const std::unordered_map<Face, std::string> &cubeMap);

 private:
	void setup();
};


#endif /* !SPIRITENGINE_SKYBOXTEXTURE_HPP */
