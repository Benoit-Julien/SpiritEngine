#ifndef SPIRITENGINE_POST_PROCESSING_HPP
#define SPIRITENGINE_POST_PROCESSING_HPP

#include <unordered_map>

#include <glad/glad.h>

#include "Singleton.hpp"
#include "Shader/ShaderProgram.hpp"

class PostProcessing : public Singleton<PostProcessing> {
	friend Singleton<PostProcessing>;

 private:
	explicit PostProcessing();
	virtual ~PostProcessing();

 private:
	GLuint _vbo;
	GLuint _vao;
	std::shared_ptr<ShaderProgram> _defaultShader;
	std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> _postProcessingMap;

 public:
	static void Draw(const GLuint &tex, const std::string &render = "");
	static void DrawDepth(const GLuint &tex);
	static void AddPostProcessing(const std::string &name, const std::string &vertexShader, const std::string &fragmentShader);

 private:
	void setup();
	void draw(const GLuint &tex);
};

#endif /* !SPIRITENGINE_POST_PROCESSING_HPP */

