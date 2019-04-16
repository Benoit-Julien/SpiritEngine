#include <iostream>
#include <fstream>
#include <sstream>

#include "AShader.hpp"

AShader::AShader(const GLenum &shaderType)
				: _shaderType(shaderType), _initialized(false) {}

AShader::~AShader() {
	if (this->_initialized)
		glDeleteShader(this->_shaderId);
}

void AShader::initFromFile(const std::string &filename) {
	this->_filename = filename;
	this->compileShader(this->loadShaderFromFile(filename));
	this->_initialized = true;
}

void AShader::initFromString(const std::string &shaderSource) {
	this->compileShader(shaderSource);
	this->_initialized = true;
}

std::string AShader::loadShaderFromFile(const std::string &filename) {
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

void AShader::compileShader(const std::string &shaderSource) {
	std::string shaderTypeString;
	switch (this->_shaderType) {
		case GL_VERTEX_SHADER:
			shaderTypeString = "GL_VERTEX_SHADER";
			break;
		case GL_FRAGMENT_SHADER:
			shaderTypeString = "GL_FRAGMENT_SHADER";
			break;
		case GL_GEOMETRY_SHADER:
			throw std::runtime_error("Geometry shaders are unsupported at this time.");
		default:
			throw std::runtime_error("Bad shader type enum in compileShader.");
	}

	// Generate a shader id
	// Note: Shader id will be non-zero if successfully created.
	GLuint shaderId = glCreateShader(this->_shaderType);
	if (shaderId == 0) {
		// Display the shader log via a runtime_error
		throw std::runtime_error("Could not create shader of type " + shaderTypeString + ": " + getInfoLog(shaderId));
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
		std::cout << shaderTypeString << " compilation failed: " << getInfoLog(shaderId) << std::endl;
	}
#ifdef NDEBUG
	else {
		std::cout << shaderTypeString << " shader compilation successful." << std::endl;
	}
#endif

	// If everything went well, return the shader id
	this->_shaderId = shaderId;
}

std::string AShader::getInfoLog(const int &id) {
	GLint infoLogLength;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

	GLchar *infoLog = new GLchar[infoLogLength + 1];
	glGetShaderInfoLog(id, infoLogLength, NULL, infoLog);

	// Convert the info log to a string
	std::string infoLogString(infoLog);

	// Delete the char array version of the log
	delete[] infoLog;

	// Finally, return the string version of the info log
	return infoLogString;
}