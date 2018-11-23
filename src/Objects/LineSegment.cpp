#include "LineSegment.hpp"

LineSegment::LineSegment() : Object(ObjectType::Line) {
	this->setup();
}

LineSegment::~LineSegment() {}

void LineSegment::draw() {
	if (!this->Enabled)
		return;

	glBindVertexArray(this->_vao); /// binding a vao
	glDrawArrays(GL_LINES, 0, 2);
}

void LineSegment::setup() {
	static glm::vec3 vertices[2] = {
					glm::vec3(0, 0, 0),
					glm::vec3(0, 1, 0)
	};

	static glm::vec4 colors[2] = {
					glm::vec4(1, 0, 0, 1),
					glm::vec4(1, 0, 0, 1)
	};

	// VAO ; Vertex Array obj
	// VBO : Vertex Buffer obj

	glGenVertexArrays(1, &this->_vao); /// Create a vao
	glBindVertexArray(this->_vao); /// "activate" or "I'm going to do something on the object"

	glGenBuffers(1, &this->_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertices); // activate it
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2, vertices, GL_STATIC_DRAW); /// Create a buffer on GPU memory and copy data
	glVertexAttribPointer( // position
					0, /// attribute number
					3, /// how many data for each vertex
					GL_FLOAT, /// data type
					GL_FALSE, /// is it normalized ?
					0, /// stride : gap between two adjacent vertex
					nullptr /// offset : gap from the starting address
	);
	glEnableVertexAttribArray(0); /// 0: attribute number

	glGenBuffers(1, &this->_vboColors);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboColors); // activate it
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, colors, GL_STATIC_DRAW); /// Create a buffer on GPU memory and copy data
	glVertexAttribPointer( // color
					1,
					4,
					GL_FLOAT,
					GL_FALSE,
					0, // gab between two vertices
					nullptr
	);
	glEnableVertexAttribArray(1); /// 1: attribute number

	glBindVertexArray(0); /// unbinding
}