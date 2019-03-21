#ifndef SPIRITENGINE_TEXTURE_HPP
#define SPIRITENGINE_TEXTURE_HPP

#include <string>
#include <glad/glad.h>

class Texture {
 public:
	enum class MinFilterEnum : GLuint {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};
	enum class MagFilterEnum : GLuint {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};
	enum class WrapEnum : GLint {
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		REPEAT = GL_REPEAT,
	};
	enum class ColorFormatEnum : GLint {
		RED = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL
	};

 private:
	std::string _filename;
	unsigned char *_image;
	int _width;
	int _height;
	int _channel;

	GLuint _tex_2d;
	bool _initialized;

 public:
	//ImageChannelEnum ImageChannel;
	MinFilterEnum MinFilter;
	MagFilterEnum MagFilter;
	WrapEnum SWrap;
	WrapEnum TWrap;
	ColorFormatEnum ColorFormat;

 public:
	explicit Texture();
	~Texture();

	Texture(const Texture &texture) = delete;
	Texture(Texture &&texture) = delete;
	Texture &operator=(const Texture &texture) = delete;
	Texture &operator=(Texture &&texture) = delete;

	inline const unsigned char *GetImage() const { return this->_image; }

	void initFromFile(const std::string &filename);

	void use();
	void disable();

 private:
	void setup();
};


#endif /* !SPIRITENGINE_TEXTURE_HPP */
