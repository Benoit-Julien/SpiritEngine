#include "SkyBoxTexture.hpp"

SkyBoxTexture::SkyBoxTexture() : Texture(GL_TEXTURE_CUBE_MAP) {
	this->_initialised = false;
}

SkyBoxTexture::~SkyBoxTexture() {}

void SkyBoxTexture::initFromCubeMap(const std::unordered_map<Face, std::string> &cubeMap) {
	this->_cubeMap = cubeMap;
	this->setup();
}

void SkyBoxTexture::setup() {
	glGenTextures(1, &this->_tex_2d);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->_tex_2d);

	for (auto &map : this->_cubeMap) {
		int channel;
		int width, height;
		auto image = Texture::loadImage(map.second.c_str(), width, height, channel);
		glTexImage2D((GLuint)map.first, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		Texture::deleteImage(image);
	}

	this->MagFilter = Texture::MagFilterEnum::LINEAR;
	this->MinFilter = Texture::MinFilterEnum::LINEAR;
	this->SWrap = this->TWrap = this->RWrap = Texture::WrapEnum::CLAMP_TO_EDGE;
	this->setTextureParameters();

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}