#ifndef SPIRITENGINE_ASHADER_HPP
#define SPIRITENGINE_ASHADER_HPP

#include <string>
#include <GL/glew.h>

class AShader {
	const GLenum _shaderType;
	GLuint _shaderId;
	std::string _filename;

	bool _initialized;

 public:
	explicit AShader(const GLenum &shaderType);
	virtual ~AShader();

	AShader(const AShader &) = delete;
	AShader(AShader &&) = delete;
	AShader &operator=(const AShader &) = delete;
	AShader &operator=(AShader &&) = delete;

	void initFromFile(const std::string &filename);
	void initFromString(const std::string &shaderSource);

	inline const GLuint &GetShaderID() const { return this->_shaderId; }
	inline const std::string &GetFilename() const { return this->_filename; }

 private:
	void compileShader(const std::string &shaderSource);
	std::string loadShaderFromFile(const std::string &filename);
	std::string getInfoLog(const int &id);
};


#endif /* !SPIRITENGINE_ASHADER_HPP */
