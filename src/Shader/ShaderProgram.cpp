#include <iostream>
#include <fstream>
#include <sstream>
#include <type_traits>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.hpp"
#include "Scene.hpp"

ShaderProgram::ShaderProgram() {
	// We start in a non-initialised state - calling initFromFiles() or initFromStrings() will
	// initialise us.
	_initialised = false;

	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating the programId or we'll segfault!
	programId = glCreateProgram();
	glUseProgram(programId);
}

ShaderProgram::~ShaderProgram() {
	// Delete the shader program from the graphics card memory to
	// free all the resources it's been using
	glDeleteProgram(programId);
}

void ShaderProgram::initFromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {
	if (this->_initialised)
		return;

	this->_vertexShader = Scene::LoadShader<VertexShader>(vertexShaderFilename);
	this->_fragmentShader = Scene::LoadShader<FragmentShader>(fragmentShaderFilename);

	this->initialise();
}

void ShaderProgram::initFromStrings(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
	if (this->_initialised)
		return;

	this->_vertexShader = std::make_shared<VertexShader>();
	this->_fragmentShader = std::make_shared<FragmentShader>();

	this->_vertexShader->initFromString(vertexShaderSource);
	this->_fragmentShader->initFromString(fragmentShaderSource);

	this->initialise();
}

void ShaderProgram::initFromShader(std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<FragmentShader> fragmentShader) {
	if (this->_initialised)
		return;

	this->_vertexShader = vertexShader;
	this->_fragmentShader = fragmentShader;

	this->initialise();
}

/**************************** BASIC TYPES ****************************/

template<>
void ShaderProgram::setUniform<int>(const std::string &uniformName, const int &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1iv(location, 1, &value);
}

template<>
void ShaderProgram::setUniform<unsigned int>(const std::string &uniformName, const unsigned int &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1uiv(location, 1, &value);
}

template<>
void ShaderProgram::setUniform<float>(const std::string &uniformName, const float &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1fv(location, 1, &value);
}

template<>
void ShaderProgram::setUniform<bool>(const std::string &uniformName, const bool &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1i(location, value);
}

/**************************** VECTOR2 ****************************/

template<>
void ShaderProgram::setUniform<glm::vec2>(const std::string &uniformName, const glm::vec2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform2fv(location, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::ivec2>(const std::string &uniformName, const glm::ivec2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform2iv(location, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::uvec2>(const std::string &uniformName, const glm::uvec2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform2uiv(location, 1, glm::value_ptr(value));
}

/**************************** VECTOR3 ****************************/

template<>
void ShaderProgram::setUniform<glm::vec3>(const std::string &uniformName, const glm::vec3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform3fv(location, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::ivec3>(const std::string &uniformName, const glm::ivec3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform3iv(location, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::uvec3>(const std::string &uniformName, const glm::uvec3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform3uiv(location, 1, glm::value_ptr(value));
}

/**************************** VECTOR4 ****************************/

template<>
void ShaderProgram::setUniform<glm::vec4>(const std::string &uniformName, const glm::vec4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform4fv(location, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::ivec4>(const std::string &uniformName, const glm::ivec4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform4iv(location, 1, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::uvec4>(const std::string &uniformName, const glm::uvec4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform4uiv(location, 1, glm::value_ptr(value));
}

/**************************** MATRIX ****************************/

template<>
void ShaderProgram::setUniform<glm::mat2>(const std::string &uniformName, const glm::mat2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::mat3>(const std::string &uniformName, const glm::mat3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void ShaderProgram::setUniform<glm::mat4>(const std::string &uniformName, const glm::mat4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

/**************************** std::variant ****************************/

template<>
void ShaderProgram::setUniform<ShaderProgram::var_t>(const std::string &uniformName, const ShaderProgram::var_t &value) {
	std::visit(overloaded{
					[this, uniformName](int arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](unsigned int arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](float arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::vec2 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::ivec2 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::uvec2 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::vec3 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::ivec3 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::uvec3 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::vec4 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::ivec4 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::uvec4 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::mat2 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::mat3 arg) { this->setUniform(uniformName, arg); },
					[this, uniformName](glm::mat4 arg) { this->setUniform(uniformName, arg); },
	}, value);
}

void ShaderProgram::initialise() {
	// Attach the compiled shaders to the shader program
	glAttachShader(programId, this->_vertexShader->GetShaderID());
	glAttachShader(programId, this->_fragmentShader->GetShaderID());

	// Link the shader program - details are placed in the program info log
	glLinkProgram(programId);

	// Once the shader program has the shaders attached and linked, the shaders are no longer required.
	// If the linking failed, then we're going to abort anyway so we still detach the shaders.
	glDetachShader(programId, this->_vertexShader->GetShaderID());
	glDetachShader(programId, this->_fragmentShader->GetShaderID());

	// Check the program link status and throw a runtime_error if program linkage failed.
	GLint programLinkSuccess = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programLinkSuccess);
#ifdef NDEBUG
	if (programLinkSuccess == GL_TRUE)
		std::cout << "Shader program link successful." << std::endl;
	else
		std::cout << "Shader program link failed: " << getInfoLog(programId) << std::endl;
#endif

	// Validate the shader program
	glValidateProgram(programId);

	// Check the validation status and throw a runtime_error if program validation failed
	GLint programValidatationStatus;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &programValidatationStatus);
#ifdef NDEBUG
	if (programValidatationStatus == GL_TRUE)
		std::cout << "Shader program validation successful." << std::endl;
	else
		std::cout << "Shader program validation failed: " << getInfoLog(programId) << std::endl;
#endif

	// Finally, the shader program is initialised
	_initialised = true;
}

std::string ShaderProgram::getInfoLog(const int &id) {
	GLint infoLogLength;
	glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

	GLchar *infoLog = new GLchar[infoLogLength + 1];
	glGetProgramInfoLog(id, infoLogLength, NULL, infoLog);

	// Convert the info log to a string
	std::string infoLogString(infoLog);

	// Delete the char array version of the log
	delete[] infoLog;

	// Finally, return the string version of the info log
	return infoLogString;
}

GLint ShaderProgram::getUniformLocation(const std::string &uniformName) {
	auto elem = this->uniforms.find(uniformName);
	if (elem != this->uniforms.end())
		return elem->second;

	GLint location = glGetUniformLocation(this->programId, uniformName.c_str());
	this->uniforms[uniformName] = location;

#ifdef NDEBUG
	if (location == -1) {
		std::cout << "Could not add uniform: " << uniformName << " - location returned -1." << std::endl;
		return -1;
	}
	std::cout << "Uniform " << uniformName << " bound to location: " << location << std::endl;
#endif
	return location;
}