#include "Texture2D.hpp"


Texture2D::Texture2D() : Texture(GL_TEXTURE_2D) {
	this->MagFilter = MagFilterEnum::LINEAR;
	this->MinFilter = MinFilterEnum::LINEAR_MIPMAP_LINEAR;
	this->SWrap = WrapEnum::MIRRORED_REPEAT;
	this->TWrap = WrapEnum::MIRRORED_REPEAT;
	this->ColorFormat = ColorFormatEnum::RGB;

	this->_initialized = false;
}

Texture2D::~Texture2D() {}

void Texture2D::initFromFile(const std::string &filename) {
	if (this->_initialized)
		return;

	this->_filename = filename;
	this->setup();
}

void Texture2D::setup() {
	auto image = Texture::loadImage(this->_filename.c_str(), this->_width, this->_height, this->_channel);

	if (this->_channel == 3 && this->ColorFormat == ColorFormatEnum::RGBA)
		this->ColorFormat = ColorFormatEnum::RGB;
	else if (this->_channel == 4 && this->ColorFormat == ColorFormatEnum::RGB)
		this->ColorFormat = ColorFormatEnum::RGBA;

	glGenTextures(1, &this->_tex);
	glBindTexture(GL_TEXTURE_2D, this->_tex);

	this->setTextureParameters();

	glTexImage2D(GL_TEXTURE_2D, 0, (GLint) this->ColorFormat, this->_width, this->_height, 0,
							 (GLenum) this->ColorFormat,
							 GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	Texture::deleteImage(image);

	this->_initialized = true;
}