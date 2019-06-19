#ifndef SPIRITENGINE_TEXTURE_HPP
#define SPIRITENGINE_TEXTURE_HPP

#include <string>
#include <glad/glad.h>

class Texture {
 public:
	enum class MinFilterEnum : GLuint {
		NONE = 0,
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};
	enum class MagFilterEnum : GLuint {
		NONE = 0,
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR
	};
	enum class WrapEnum : GLint {
		NONE = 0,
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

 protected:
	GLuint _tex;
	bool _initialized;

 private:
	const GLenum _type;

 public:
	MinFilterEnum MinFilter;
	MagFilterEnum MagFilter;
	WrapEnum SWrap;
	WrapEnum TWrap;
	WrapEnum RWrap;
	ColorFormatEnum ColorFormat;

 public:
	explicit Texture(const GLenum &type);
	virtual ~Texture();

	Texture(const Texture &texture) = delete;
	Texture(Texture &&texture) = delete;
	Texture &operator=(const Texture &texture) = delete;
	Texture &operator=(Texture &&texture) = delete;

	virtual void use();
	virtual void disable();

 protected:
	void setTextureParameters() const;

	static unsigned char *loadImage(const std::string &filename, int &width, int &height, int &channel, const int &desired_channels = 0);
	static void deleteImage(unsigned char *image);
};


#endif /* !SPIRITENGINE_TEXTURE_HPP */
