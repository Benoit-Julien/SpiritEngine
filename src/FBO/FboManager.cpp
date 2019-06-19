#include <iostream>
#include "FboManager.hpp"

FboManager::FboManager() {
	useRenderDepthBuffer = false;
	attachmentCount = 0;
	/*for(int i=0;i<5;i++) {
		mrt[i] = GL_COLOR_ATTACHMENT0+i;
	}*/
	glGenFramebuffers(1, &_fboId);
}

FboManager::~FboManager() {
	glDeleteFramebuffers(1, &_fboId);
	if (useRenderDepthBuffer) {
		glDeleteRenderbuffers(1, &_renderDepthBufferId);
	}
}

//generate and bind depth buffer
void FboManager::genRenderDepthBuffer(const unsigned &w, const unsigned &h) {
	glGenRenderbuffers(1, &this->_renderDepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, this->_renderDepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

//bind depth buffer
void FboManager::bindRenderDepthBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fboId);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->_renderDepthBufferId);
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FboManager::setDrawBuffers() {
	glDrawBuffers(this->attachmentCount, this->mrt);
}

void FboManager::bindToFbo(const GLenum &type, const GLenum &texture, const GLuint &textureId) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fboId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, type, texture, textureId, 0);
	if (type != GL_DEPTH_ATTACHMENT) {
		this->mrt[this->attachmentCount] = type;
		this->attachmentCount += 1;
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	//glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FboManager::bind3DTextureToFbo(const GLenum &type, const GLuint &textureId) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->_fboId);
	glFramebufferTexture(GL_FRAMEBUFFER, type, textureId, 0);
	//glFramebufferTexture3D(GL_FRAMEBUFFER, type, GL_TEXTURE_3D, textureId, 0, 0);
	if (type != GL_DEPTH_ATTACHMENT) {
		this->mrt[this->attachmentCount] = type;
		this->attachmentCount += 1;
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
}

bool FboManager::checkFboStatus() {
	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE:
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cerr << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cerr << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cerr << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cerr << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cerr << "[ERROR] Unsupported by FBO implementation." << std::endl;
			return false;

		default:
			std::cerr << "[ERROR] Unknow error." << std::endl;
			return false;
	}
}