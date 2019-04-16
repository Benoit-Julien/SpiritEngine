#include <exception>

#include "PostProcessing.hpp"
#include "FBO/TextureViewerShader.hpp"

PostProcessing::PostProcessing() {
	this->setup();
}

PostProcessing::~PostProcessing() {
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_vao);
}

void PostProcessing::Draw(const GLuint &tex, const std::string &render) {
	auto self = PostProcessing::getSingletonPtr();

	auto shader = (render.empty()) ? self->_defaultShader : self->_postProcessingMap[render];

	shader->use();
	if (render.empty())
		shader->setUniform("depth", false);

	self->draw(tex);

	shader->disable();
}

void PostProcessing::DrawDepth(const GLuint &tex) {
	auto self = PostProcessing::getSingletonPtr();

	self->_defaultShader->use();
	self->_defaultShader->setUniform("depth", true);

	self->draw(tex);

	self->_defaultShader->disable();
}

void PostProcessing::AddPostProcessing(const std::string &name, const std::string &vertexShader, const std::string &fragmentShader) {
	auto self = PostProcessing::getSingletonPtr();

	if (self->_postProcessingMap.find(name) != self->_postProcessingMap.end())
		throw std::logic_error("Cannot add Post Processing effect " + name + " because it already exist.");

	auto shader = std::make_shared<ShaderProgram>();
	shader->initFromFiles(vertexShader, fragmentShader);

	shader->use();
	shader->setUniform("tex", 0);
	shader->disable();

	self->_postProcessingMap[name] = shader;
}

//Called only once in constructor
void PostProcessing::setup() {
	this->_defaultShader = std::make_shared<ShaderProgram>();
	this->_defaultShader->initFromStrings(TextureViewerVertexShader, TextureViewerFragmentShader);

	this->_defaultShader->use();
	this->_defaultShader->setUniform("tex", 0);
	this->_defaultShader->disable();

	//Quad verticles - omitted z coord, because it will always be 1
	float pos[] = {
					-1.0, 1.0,
					1.0, 1.0,
					-1.0, -1.0,
					1.0, -1.0
	};

	float uv[] = {
					0.0, 1.0,
					1.0, 1.0,
					0.0, 0.0,
					1.0, 0.0
	};

	//Generate VAO
	glGenVertexArrays(1, &this->_vao);
	//Bind VAO
	glBindVertexArray(this->_vao);

	//Generate VBO
	glGenBuffers(1, &this->_vbo);
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
	//Alocate buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos) + sizeof(uv), NULL, GL_STATIC_DRAW);
	//Fill VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(pos), pos);
	glBufferSubData(GL_ARRAY_BUFFER, 8 * sizeof(float), sizeof(uv), uv);

	//Fill attributes and uniforms
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), (void *) nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), (GLvoid *) (sizeof(float) * 8));

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void PostProcessing::draw(const GLuint &tex) {
	//Bind VAO
	glBindVertexArray(this->_vao);
	//Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);

	//Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	//Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	//Unbind texture for sure
	glBindTexture(GL_TEXTURE_2D, 0);

	//Unbind VBO - just for sure
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Unbind VAO
	glBindVertexArray(0);
}