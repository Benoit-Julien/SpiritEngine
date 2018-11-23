#include "TriangleObject.hpp"

TriangleObject::TriangleObject()
				: Drawable(ObjectType::Triangle),
					_drawMode(DrawMode::Shape) {
}

TriangleObject::TriangleObject(const ObjectType &type)
				: Drawable(type),
					_drawMode(DrawMode::Shape) {
}

TriangleObject::~TriangleObject() {

}

TriangleObject::TriangleObject(const TriangleObject &object)
				: Drawable(object),
					vertices(object.vertices),
					normals(object.normals),
					triangles(object.triangles),
					_drawMode(object._drawMode) {
	this->setup();
}

TriangleObject &TriangleObject::operator=(const TriangleObject &object) {
	this->vertices = object.vertices;
	this->normals = object.normals;
	this->triangles = object.triangles;
	this->_drawMode = object._drawMode;
	this->setup();
	return *this;
}

void TriangleObject::Draw() {
	if (!this->Enabled())
		return;

	glBindVertexArray(this->_vao); /// binding a vao

	if (this->_drawMode == DrawMode::WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, nullptr);

	if (this->_drawMode == DrawMode::WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void TriangleObject::setup() {
	// VAO ; Vertex Array obj
	// VBO : Vertex Buffer obj

	glGenVertexArrays(1, &this->_vao); /// Create a vao
	glBindVertexArray(this->_vao); /// "activate" or "I'm going to do something on the object"

	glGenBuffers(1, &this->_vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboVertices); // activate it
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW); /// Create a buffer on GPU memory and copy data
	glVertexAttribPointer( // position
					0, /// attribute number
					3, /// how many data for each vertex
					GL_FLOAT, /// data type
					GL_FALSE, /// is it normalized ?
					0, /// stride : gap between two adjacent vertex
					nullptr /// offset : gap from the starting address
	);
	glEnableVertexAttribArray(0); /// 0: attribute number

	glGenBuffers(1, &this->_vboNormals);
	glBindBuffer(GL_ARRAY_BUFFER, this->_vboNormals); // activate it
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * this->normals.size(), this->normals.data(), GL_STATIC_DRAW); /// Create a buffer on GPU memory and copy data
	glVertexAttribPointer( // color
					1,
					3,
					GL_FLOAT,
					GL_FALSE,
					0, // gab between two vertices
					nullptr
	);
	glEnableVertexAttribArray(1); /// 1: attribute number

	glGenBuffers(1, &this->_iboElements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_iboElements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriangleIndex) * this->triangles.size(), this->triangles.data(), GL_STATIC_DRAW);

	glBindVertexArray(0); /// unbinding
}