#include "CullFace.hpp"

CullFace::CullFace() {
	this->_enabledCulling = false;
	this->_cullface = GL_BACK;
}

CullFace::CullFace(const CullFace &cull) {
	this->_enabledCulling = cull._enabledCulling;
	this->_cullface = cull._cullface;
}

CullFace &CullFace::operator=(const CullFace &cull) {
	this->_enabledCulling = cull._enabledCulling;
	this->_cullface = cull._cullface;

	return *this;
}

CullFace::~CullFace() {}

void CullFace::SetCullFaceOption(const GLenum &cullface) {
	this->_cullface = cullface;
}

void CullFace::EnableCulling() {
	this->_enabledCulling = true;
}

void CullFace::DisableCulling() {
	this->_enabledCulling = false;
}
