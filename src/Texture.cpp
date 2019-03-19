#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

Texture::Texture() {
	this->MagFilter = MagFilterEnum::LINEAR;
	this->MinFilter = MinFilterEnum::LINEAR_MIPMAP_LINEAR;
	this->SWrap = WrapEnum::MIRRORED_REPEAT;
	this->TWrap = WrapEnum::MIRRORED_REPEAT;
	this->ColorFormat = ColorFormatEnum::RGB;
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

void Texture::initFromFile(const std::string &filename) {
	this->_filename = filename;
	this->setup();
}

void Texture::setup() {
	this->_image = stbi_load(this->_filename.c_str(), &this->_width, &this->_height, &this->_channel, 0);

	glGenTextures(1, &this->_tex_2d);
	glBindTexture(GL_TEXTURE_2D, this->_tex_2d);

	// When we need to maje the texture bigger
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLfloat) this->MagFilter);
	// When we need to make the texture smaller
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLfloat) this->MinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint) this->SWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint) this->TWrap);

	glTexImage2D(GL_TEXTURE_2D, 0, (GLint) this->ColorFormat, this->_width, this->_height, 0,
							 (GLenum) this->ColorFormat,
							 GL_UNSIGNED_BYTE, this->_image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}