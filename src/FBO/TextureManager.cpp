#include "TextureManager.hpp"

#define MAX_GRID_SIZE 32

TextureManager::TextureManager() : _clearTextureExtension(true) {
}

TextureManager::~TextureManager() {
	for (auto &tex : this->_textures)
		glDeleteTextures(1, &tex.second);
	_textures.clear();
}

void TextureManager::Add(const std::string &texture) {
	auto elem = this->_textures.find(texture);
	if (elem != this->_textures.end())
		glDeleteTextures(1, &elem->second);

	GLuint tmpId;
	glGenTextures(1, &tmpId);
	this->_textures[texture] = tmpId;
}

void TextureManager::CreateTexture(const std::string &texture, const unsigned &w, const unsigned &h, const GLuint &filter, const GLuint &type, const GLuint &type_2, const bool &depth) {
	//	SDL_Surface *surface;
	GLuint textureid;
	int mode;
	Add(texture);
	glBindTexture(GL_TEXTURE_2D, this->_textures[texture]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//No image data - for frameBuffer
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type_2, GL_FLOAT, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type_2, GL_FLOAT, 0);
	if (depth) {

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
	}
}

void TextureManager::CreateRGBA16F3DTexture(const std::string &texture, const glm::vec3 &dim, const GLuint &filter, const GLuint &wrap) {
	GLuint tex;
	Add(texture);
	glBindTexture(GL_TEXTURE_3D, this->_textures[texture]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, w, h, d, 0, GL_RGBA, GL_FLOAT, NULL);
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, w, h, d, 0, GL_RGBA, GL_FLOAT, &emptyData[0]);
	std::vector<GLfloat> emptyData(dim.x * dim.y * dim.z * sizeof(float), 0.0);
	glBindTexture(GL_TEXTURE_3D, this->_textures[texture]);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, dim.x, dim.y, dim.z, 0, GL_RGBA, GL_FLOAT, &emptyData[0]);
	glBindTexture(GL_TEXTURE_3D, 0);
	//clear3Dtexture(textures[texture], dim);
	//glBindTexture(GL_TEXTURE_3D, 0);
}

void TextureManager::CreateRGBA3DTexture(const std::string &texture, const glm::vec3 &dim, const GLuint &filter, const GLuint &wrap) {
	GLuint tex;
	Add(texture);
	glBindTexture(GL_TEXTURE_3D, this->_textures[texture]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrap);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, dim.x, dim.y, dim.z, 0, GL_RGBA, GL_FLOAT, NULL);
	//glBindTexture(GL_TEXTURE_3D, 0);
}

void TextureManager::Clear3Dtexture(const GLuint &texture) {
	if (this->_clearTextureExtension) {
		GLfloat data[4] = {0.0f, 0.0f, 0.0f, 0.0f};
		//glClearTexImage(texture, 0, GL_RGBA, GL_FLOAT, &data[0]);
	}
	else {
		//MUCH SLOWER version, but should work on version lower than 4.4
		std::vector<GLfloat> emptyData(MAX_GRID_SIZE * MAX_GRID_SIZE * MAX_GRID_SIZE * sizeof(float), 0.0);
		glBindTexture(GL_TEXTURE_3D, texture);
		//glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, MAX_GRID_SIZE, MAX_GRID_SIZE, MAX_GRID_SIZE, 0, GL_RGBA, GL_FLOAT, &emptyData[0]);
		//or
		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, MAX_GRID_SIZE, MAX_GRID_SIZE, MAX_GRID_SIZE, GL_RGBA, GL_FLOAT, &emptyData[0]);
		glBindTexture(GL_TEXTURE_3D, 0);
	}
}