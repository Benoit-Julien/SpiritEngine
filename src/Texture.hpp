#ifndef SPIRITENGINE_TEXTURE_HPP
#define SPIRITENGINE_TEXTURE_HPP

#include <string>
#include <GL/glew.h>

class Texture {
	unsigned char *_image;
	int _width;
	int _height;
	int _channel;

	GLuint _tex_2d;

 public:
	explicit Texture(const std::string &filename);
	~Texture();

	Texture(const Texture &texture) = delete;
	Texture(Texture &&texture) = delete;
	Texture &operator=(const Texture &texture) = delete;
	Texture &operator=(Texture &&texture) = delete;

	inline const unsigned char *GetImage() const { return this->_image; }

	void use();
	void disable();
};


#endif /* !SPIRITENGINE_TEXTURE_HPP */
