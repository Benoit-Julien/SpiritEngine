#ifndef SPIRITENGINE_DEFAULTSHADER_HPP
#define SPIRITENGINE_DEFAULTSHADER_HPP

#include "ShaderProgram.hpp"

class GBufferShader : public ShaderProgram
{
 public:
	static const char *VERTEX_SHADER;
	static const char *FRAGMENT_SHADER;

 public:
	explicit GBufferShader();
	~GBufferShader() = default;
};

class DeferredShader : public ShaderProgram
{
 public:
	static const char *VERTEX_SHADER;
	static const char *FRAGMENT_SHADER;

 public:
	explicit DeferredShader();
	~DeferredShader() = default;
};

class TextureViewerShader : public ShaderProgram
{
 public:
	static const char *VERTEX_SHADER;
	static const char *FRAGMENT_SHADER;

 public:
	explicit TextureViewerShader();
	~TextureViewerShader() = default;
};

#endif /* !SPIRITENGINE_DEFAULTSHADER_HPP */
