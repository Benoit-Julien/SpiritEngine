#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stb/stb_image.h>

Texture::Texture(const GLenum &type) : _type(type) {
	this->MagFilter = MagFilterEnum::NONE;
	this->MinFilter = MinFilterEnum::NONE;
	this->SWrap = WrapEnum::NONE;
	this->TWrap = WrapEnum::NONE;
	this->RWrap = WrapEnum::NONE;
	this->ColorFormat = ColorFormatEnum::RGB;
}

Texture::~Texture() {
	glDeleteTextures(1, &this->_tex);
}

void Texture::use() {
	glBindTexture(this->_type, this->_tex);
}

void Texture::disable() {
	glBindTexture(this->_type, 0);
}

void Texture::setTextureParameters() const {
	// When we need to maje the texture bigger
	if (this->MagFilter != MagFilterEnum::NONE)
		glTexParameterf(this->_type, GL_TEXTURE_MAG_FILTER, (GLfloat) this->MagFilter);
	// When we need to make the texture smaller
	if (this->MinFilter != MinFilterEnum::NONE)
		glTexParameterf(this->_type, GL_TEXTURE_MIN_FILTER, (GLfloat) this->MinFilter);
	if (this->SWrap != WrapEnum::NONE)
		glTexParameteri(this->_type, GL_TEXTURE_WRAP_S, (GLint) this->SWrap);
	if (this->TWrap != WrapEnum::NONE)
		glTexParameteri(this->_type, GL_TEXTURE_WRAP_T, (GLint) this->TWrap);
	if (this->RWrap != WrapEnum::NONE)
		glTexParameteri(this->_type, GL_TEXTURE_WRAP_R, (GLint) this->RWrap);
}

unsigned char *Texture::loadImage(const std::string &filename, int &width, int &height, int &channel, const int &desired_channels) {
	unsigned char *image = stbi_load(filename.c_str(), &width, &height, &channel, desired_channels);
	if (!image)
		throw std::logic_error("Fail create texture based on " + filename);

#ifdef NDEBUG
	std::cout << "Texture image : " << filename << " loaded." << std::endl;
#endif
	return image;
}

void Texture::deleteImage(unsigned char *image) {
	stbi_image_free(image);
}