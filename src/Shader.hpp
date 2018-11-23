#ifndef COMPUTERGRAPHICS1_SHADER_HPP
#define COMPUTERGRAPHICS1_SHADER_HPP

#include <unordered_map>
#include <string>
#include <GL/glew.h>

class Shader {
 private:
	// static DEBUG flag - if set to false then, errors aside, we'll run completely silent
	static const bool DEBUG = true;

	// We'll use an enum to differentiate between shaders and shader programs when querying the info log
	enum class ObjectType {
		SHADER, PROGRAM
	};

	// Shader program and individual shader Ids
	GLuint programId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;

	// Has this shader program been initialised?
	bool initialised;

	std::unordered_map<std::string, GLint> uniforms;

 public:
	Shader();
	~Shader();
	Shader(const Shader &shader) = delete;
	Shader(Shader &&) = delete;
	Shader &operator=(const Shader &shader) = delete;
	Shader &operator=(Shader &&) = delete;

	// Method to initialise a shader program from shaders provided as files
	void initFromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);

	// Method to initialise a shader program from shaders provided as strings
	void initFromStrings(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

	// Method to enable the shader program - we'll suggest this for inlining
	inline void use() {
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
	void setUniform(const std::string &uniformName, const T &value) {}

 private:
	// Private method to compile a shader of a given type
	GLuint compileShader(const std::string &shaderSource, const GLenum &shaderType);

	// Private method to compile/attach/link/verify the shaders.
	// Note: Rather than returning a boolean as a success/fail status we'll just consider
	// a failure here to be an unrecoverable error and throw a runtime_error.
	void initialise(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);

	// Private method to load the shader source code from a file
	std::string loadShaderFromFile(const std::string &filename);

	// Private method to return the current shader program info log as a string
	std::string getInfoLog(const ObjectType &type, const int &id);

	GLint getUniformLocation(const std::string &uniformName);
};

#endif /* !COMPUTERGRAPHICS1_SHADER_HPP */
