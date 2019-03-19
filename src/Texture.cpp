#include "Texture.hpp"

#include <stb/stb_image.h>

Texture::Texture(const std::string &filename) : _filename(filename) {
	this->setup();
}

Texture::~Texture() {
	glDeleteTextures(1, &this->_tex_2d);
	stbi_image_free(this->_image);
}

void Texture::use() {
	glBindTexture(GL_TEXTURE_2D, this->_tex_2d);
}

void Texture::disable() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setup() {
	this->_image = stbi_load(this->_filename.c_str(), &this->_width, &this->_height, &this->_channel, 0);

	glGenTextures(1, &this->_tex_2d);
	glBindTexture(GL_TEXTURE_2D, this->_tex_2d);

	// When we need to maje the texture bigger
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// When we need to make the texture smaller
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->_width, this->_height, 0, GL_RGB,
							 GL_UNSIGNED_BYTE, this->_image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}