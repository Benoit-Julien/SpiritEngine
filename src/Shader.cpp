#include <iostream>
#include <fstream>
#include <sstream>
#include <type_traits>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"

Shader::Shader() {
	// We start in a non-initialised state - calling initFromFiles() or initFromStrings() will
	// initialise us.
	initialised = false;

	// Generate a unique Id / handle for the shader program
	// Note: We MUST have a valid rendering context before generating the programId or we'll segfault!
	programId = glCreateProgram();
	glUseProgram(programId);
}

Shader::~Shader() {
	// Delete the shader program from the graphics card memory to
	// free all the resources it's been using
	glDeleteProgram(programId);
}

void Shader::initFromFiles(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {
	// Get the shader file contents as strings
	std::string vertexShaderSource = loadShaderFromFile(vertexShaderFilename);
	std::string fragmentShaderSource = loadShaderFromFile(fragmentShaderFilename);

	initialise(vertexShaderSource, fragmentShaderSource);
}

void Shader::initFromStrings(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
	initialise(vertexShaderSource, fragmentShaderSource);
}

/**************************** BASIC TYPES ****************************/

template<>
void Shader::setUniform<int>(const std::string &uniformName, const int &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1iv(location, 1, &value);
}

template<>
void Shader::setUniform<unsigned int>(const std::string &uniformName, const unsigned int &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1uiv(location, 1, &value);
}

template<>
void Shader::setUniform<float>(const std::string &uniformName, const float &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform1fv(location, 1, &value);
}

/**************************** VECTOR2 ****************************/

template<>
void Shader::setUniform<glm::vec2>(const std::string &uniformName, const glm::vec2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform2fv(location, 1, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::ivec2>(const std::string &uniformName, const glm::ivec2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform2iv(location, 1, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::uvec2>(const std::string &uniformName, const glm::uvec2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform2uiv(location, 1, glm::value_ptr(value));
}

/**************************** VECTOR3 ****************************/

template<>
void Shader::setUniform<glm::vec3>(const std::string &uniformName, const glm::vec3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform3fv(location, 1, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::ivec3>(const std::string &uniformName, const glm::ivec3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform3iv(location, 1, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::uvec3>(const std::string &uniformName, const glm::uvec3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform3uiv(location, 1, glm::value_ptr(value));
}

/**************************** VECTOR4 ****************************/

template<>
void Shader::setUniform<glm::vec4>(const std::string &uniformName, const glm::vec4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform4fv(location, 1, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::ivec4>(const std::string &uniformName, const glm::ivec4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform4iv(location, 1, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::uvec4>(const std::string &uniformName, const glm::uvec4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniform4uiv(location, 1, glm::value_ptr(value));
}

/**************************** MATRIX ****************************/

template<>
void Shader::setUniform<glm::mat2>(const std::string &uniformName, const glm::mat2 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::mat3>(const std::string &uniformName, const glm::mat3 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void Shader::setUniform<glm::mat4>(const std::string &uniformName, const glm::mat4 &value) {
	GLint location = this->getUniformLocation(uniformName);
	if (location == -1) return;
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

/**************************** std::variant ****************************/

template<>
void Shader::setUniform<Shader::var_t>(const std::string &uniformName, const Shader::var_t &value) {
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

GLuint Shader::compileShader(const std::string &shaderSource, const GLenum &shaderType) {
	std::string shaderTypeString;
	switch (shaderType) {
		case GL_VERTEX_SHADER:
			shaderTypeString = "GL_VERTEX_SHADER";
			break;
		case GL_FRAGMENT_SHADER:
			shaderTypeString = "GL_FRAGMENT_SHADER";
			break;
		case GL_GEOMETRY_SHADER:
			throw std::runtime_error("Geometry shaders are unsupported at this time.");
			break;
		default:
			throw std::runtime_error("Bad shader type enum in compileShader.");
			break;
	}

	// Generate a shader id
	// Note: Shader id will be non-zero if successfully created.
	GLuint shaderId = glCreateShader(shaderType);
	if (shaderId == 0) {
		// Display the shader log via a runtime_error
		throw std::runtime_error("Could not create shader of type " + shaderTypeString + ": " + getInfoLog(ObjectType::SHADER, shaderId));
	}

	// Get the source string as a pointer to an array of characters
	const char *shaderSourceChars = shaderSource.c_str();

	// Attach the GLSL source code to the shader
	// Params: GLuint shader, GLsizei count, const GLchar **string, const GLint *length
	// Note: The pointer to an array of source chars will be null terminated, so we don't need to specify the length and can instead use NULL.
	glShaderSource(shaderId, 1, &shaderSourceChars, NULL);

	// Compile the shader
	glCompileShader(shaderId);

	// Check the compilation status and throw a runtime_error if shader compilation failed
	GLint shaderStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderStatus);
	if (shaderStatus == GL_FALSE) {
		std::cout << shaderTypeString << " compilation failed: " << getInfoLog(ObjectType::SHADER, shaderId) << std::endl;
	}
	else {
		if (DEBUG) {
			std::cout << shaderTypeString << " shader compilation successful." << std::endl;
		}
	}

	// If everything went well, return the shader id
	return shaderId;
}

void Shader::initialise(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
	// Compile the shaders and return their id values
	vertexShaderId = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
	fragmentShaderId = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	// Attach the compiled shaders to the shader program
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);

	// Link the shader program - details are placed in the program info log
	glLinkProgram(programId);

	// Once the shader program has the shaders attached and linked, the shaders are no longer required.
	// If the linking failed, then we're going to abort anyway so we still detach the shaders.
	glDetachShader(programId, vertexShaderId);
	glDetachShader(programId, fragmentShaderId);

	// Check the program link status and throw a runtime_error if program linkage failed.
	GLint programLinkSuccess = GL_FALSE;
	glGetProgramiv(programId, GL_LINK_STATUS, &programLinkSuccess);
	if (programLinkSuccess == GL_TRUE) {
		if (DEBUG) {
			std::cout << "Shader program link successful." << std::endl;
		}
	}
	else {
		std::cout << "Shader program link failed: " << getInfoLog(ObjectType::PROGRAM, programId) << std::endl;
	}

	// Validate the shader program
	glValidateProgram(programId);

	// Check the validation status and throw a runtime_error if program validation failed
	GLint programValidatationStatus;
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &programValidatationStatus);
	if (programValidatationStatus == GL_TRUE) {
		if (DEBUG) {
			std::cout << "Shader program validation successful." << std::endl;
		}
	}
	else {
		std::cout << "Shader program validation failed: " << getInfoLog(ObjectType::PROGRAM, programId) << std::endl;
	}

	// Finally, the shader program is initialised
	initialised = true;
}

std::string Shader::loadShaderFromFile(const std::string &filename) {
	// Create an input filestream and attempt to open the specified file
	std::ifstream file(filename.c_str());

	// If we couldn't open the file we'll bail out
	if (!file.good())
		throw std::runtime_error("Failed to open file: " + filename);

	// Otherwise, create a string stream...
	std::stringstream stream;

	// ...and dump the contents of the file into it.
	stream << file.rdbuf();

	// Now that we've read the file we can close it
	file.close();

	// Finally, convert the stringstream into a string and return it
	return stream.str();
}

std::string Shader::getInfoLog(const Shader::ObjectType &type, const int &id) {
	GLint infoLogLength;
	if (type == ObjectType::SHADER) {
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	}
	else // type must be ObjectType::PROGRAM
	{
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	}

	GLchar *infoLog = new GLchar[infoLogLength + 1];
	if (type == ObjectType::SHADER) {
		glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);
	}
	else // type must be ObjectType::PROGRAM
	{
		glGetProgramInfoLog(id, infoLogLength, NULL, infoLog);
	}

	// Convert the info log to a string
	std::string infoLogString(infoLog);

	// Delete the char array version of the log
	delete[] infoLog;

	// Finally, return the string version of the info log
	return infoLogString;
}

GLint Shader::getUniformLocation(const std::string &uniformName) {
	auto elem = this->uniforms.find(uniformName);
	if (elem != this->uniforms.end())
		return elem->second;

	GLint location = glGetUniformLocation(this->programId, uniformName.c_str());
	this->uniforms[uniformName] = location;

	if (location == -1) {
		std::cout << "Could not add uniform: " << uniformName << " - location returned -1." << std::endl;
		return -1;
	}
	if (DEBUG) std::cout << "Uniform " << uniformName << " bound to location: " << location << std::endl;
	return location;
}