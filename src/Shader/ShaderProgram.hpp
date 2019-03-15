#ifndef COMPUTERGRAPHICS1_SHADER_HPP
#define COMPUTERGRAPHICS1_SHADER_HPP

#include <unordered_map>
#include <memory>
#include <iostream>
#include <variant>
#include <string>

#include <glad/glad.h>

#include "VertexShader.hpp"
#include "FragmentShader.hpp"

class ShaderProgram {
 private:
	// Shader program and individual shader Ids
	GLuint programId;
	std::shared_ptr<VertexShader> _vertexShader;
	std::shared_ptr<FragmentShader> _fragmentShader;

	// Has this shader program been initialised?
	bool initialised;

	std::unordered_map<std::string, GLint> uniforms;

	template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
	template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

 public:
	typedef std::variant<
	        int,
	        unsigned int,
	        float,
	        glm::vec2,
					glm::ivec2,
					glm::uvec2,
					glm::vec3,
					glm::ivec3,
					glm::uvec3,
					glm::vec4,
					glm::ivec4,
					glm::uvec4,
					glm::mat2,
					glm::mat3,
					glm::mat4> var_t;

 public:
	ShaderProgram();
	~ShaderProgram();

	ShaderProgram(const ShaderProgram &shader) = delete;
	ShaderProgram(ShaderProgram &&) = delete;
	ShaderProgram &operator=(const ShaderProgram &shader) = delete;
	ShaderProgram &operator=(ShaderProgram &&) = delete;

	// Method to initialise a shader program from shaders provided as files
	void initFromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);

	// Method to initialise a shader program from shaders provided as strings
	void initFromStrings(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

	// Method to enable the shader program - we'll suggest this for inlining
	void use() {
		// Santity check that we're initialised and ready to go...
		if (initialised) {
			glUseProgram(programId);
		}
		else {
			std::string msg = "Shader program " + std::to_string(programId);
			msg += " not initialised - aborting.";
			throw std::runtime_error(msg);
		}
	}

	// Method to disable the shader - we'll also suggest this for inlining
	inline void disable() {
		glUseProgram(0);
	}

	template<typename T>
	void setUniform(const std::string &uniformName, const T &value);

 private:
	// Private method to compile/attach/link/verify the shaders.
	// Note: Rather than returning a boolean as a success/fail status we'll just consider
	// a failure here to be an unrecoverable error and throw a runtime_error.
	void initialise();

	// Private method to return the current shader program info log as a string
	std::string getInfoLog(const int &id);

	GLint getUniformLocation(const std::string &uniformName);
};

#endif /* !COMPUTERGRAPHICS1_SHADER_HPP */
