#ifndef SPIRITENGINE_DEFERREDSHADING_HPP
#define SPIRITENGINE_DEFERREDSHADING_HPP

#include <memory>

#include "Singleton.hpp"
#include "FBO/TextureManager.hpp"
#include "FBO/FboManager.hpp"
#include "Scene.hpp"

class DeferredShading : public Singleton<DeferredShading> {
	friend Singleton<DeferredShading>;

 private:
	GLuint _vbo;
	GLuint _vao;

	int _width;
	int _height;

	float _constant;
	float _linear;
	float _quadratic;

	std::shared_ptr<TextureManager> _texManager;
	std::shared_ptr<FboManager> _gbufferFboManager;
	std::shared_ptr<FboManager> _deferredFboManager;

	std::shared_ptr<ShaderProgram> _shader;

 private:
	explicit DeferredShading();
	~DeferredShading();

 public:
	static void WindowResize(const int &width, const int &height);
	static void Draw(const DrawInformation &info, const bool &doLightPass = true);

	inline static const GLuint &GetTexture(const std::string &texture) {
		return DeferredShading::getSingleton()._texManager->Get(texture);
	}

 private:
	void setupTexture();
	void setup();

	void drawQuad();
	void setTexture(const std::string &name, const GLuint &texture, const int &index);
};

#endif /* !SPIRITENGINE_DEFERREDSHADING_HPP */
