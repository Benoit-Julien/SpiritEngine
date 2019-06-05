#include "DefaultShader.hpp"

const char *GBufferShader::VERTEX_SHADER =

#include "glsl/GBuffer.vert"

const char *GBufferShader::FRAGMENT_SHADER =

#include "glsl/GBuffer.frag"

const char *DeferredShader::VERTEX_SHADER =

#include "glsl/deferred.vert"

const char *DeferredShader::FRAGMENT_SHADER =

#include "glsl/deferred.frag"

const char *TextureViewerShader::VERTEX_SHADER =

#include "glsl/textureViewer.vert"

const char *TextureViewerShader::FRAGMENT_SHADER =

#include "glsl/textureViewer.frag"

GBufferShader::GBufferShader() {
	this->initFromStrings(GBufferShader::VERTEX_SHADER, GBufferShader::FRAGMENT_SHADER);
}

DeferredShader::DeferredShader() {
	this->initFromStrings(DeferredShader::VERTEX_SHADER, DeferredShader::FRAGMENT_SHADER);
}

TextureViewerShader::TextureViewerShader() {
	this->initFromStrings(TextureViewerShader::VERTEX_SHADER, TextureViewerShader::FRAGMENT_SHADER);
}