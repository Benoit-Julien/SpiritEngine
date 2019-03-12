#include "TriangleObject.hpp"
#include "Objects/Lights/Light.hpp"

TriangleObject::TriangleObject()
				: Drawable(ObjectType::Triangle),
					useOwnMaterial(true),
					_drawMode(DrawMode::Shape) {
}

TriangleObject::TriangleObject(const ObjectType &type)
				: Drawable(type),
					useOwnMaterial(true),
					_drawMode(DrawMode::Shape) {
}

TriangleObject::~TriangleObject() {
	if (this->_vbo[VERTEX_BUFFER] != 0)
		glDeleteBuffers(1, &this->_vbo[VERTEX_BUFFER]);
	if (this->_vbo[TEXCOORD_BUFFER] != 0)
		glDeleteBuffers(1, &this->_vbo[TEXCOORD_BUFFER]);
	if (this->_vbo[NORMAL_BUFFER] != 0)
		glDeleteBuffers(1, &this->_vbo[NORMAL_BUFFER]);
	if (this->_vbo[VERTEX_BUFFER] != 0)
		glDeleteBuffers(1, &this->_vbo[INDEX_BUFFER]);

	glDeleteVertexArrays(1, &this->_vao);
}

TriangleObject::TriangleObject(const TriangleObject &object)
				: Drawable(object),
					vertices(object.vertices),
					normals(object.normals),
					triangles(object.triangles),
					useOwnMaterial(object.useOwnMaterial),
					_drawMode(object._drawMode) {
	this->setup();
}

TriangleObject &TriangleObject::operator=(const TriangleObject &object) {
	this->useOwnMaterial = object.useOwnMaterial;
	this->vertices = object.vertices;
	this->normals = object.normals;
	this->triangles = object.triangles;
	this->_drawMode = object._drawMode;
	this->setup();
	return *this;
}
void TriangleObject::Draw(const ShaderVariables &variables) {
	if (!this->Enabled())
		return;

	if (this->Culling()) {
		glEnable(GL_CULL_FACE);
		glCullFace(this->GetCullFaceOption());
	}

	if (this->useOwnMaterial) {
		this->material->use();
		this->updateShaderUniform(variables);
	}

	glBindVertexArray(this->_vao); /// binding a vao

	if (this->_drawMode == DrawMode::WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLuint), GL_UNSIGNED_INT, nullptr);

	if (this->_drawMode == DrawMode::WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (this->useOwnMaterial) this->material->disable();
	if (this->Culling()) glDisable(GL_CULL_FACE);
}

void TriangleObject::setup() {
	// VAO ; Vertex Array obj
	// VBO : Vertex Buffer obj

	glGenVertexArrays(1, &this->_vao); /// Create a vao
	glBindVertexArray(this->_vao); /// "activate" or "I'm going to do something on the object"

	if (!this->vertices.empty()) {
		glGenBuffers(1, &this->_vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[VERTEX_BUFFER]); // activate it
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * this->vertices.size(), this->vertices.data(),
								 GL_STATIC_DRAW); /// Create a buffer on GPU memory and copy data
		glVertexAttribPointer( // position
						0, /// attribute number
						3, /// how many data for each vertex
						GL_FLOAT, /// data type
						GL_FALSE, /// is it normalized ?
						0, /// stride : gap between two adjacent vertex
						nullptr /// offset : gap from the starting address
		);
		glEnableVertexAttribArray(0); /// 0: attribute number
	}

	if (!this->normals.empty()) {
		glGenBuffers(1, &this->_vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[NORMAL_BUFFER]); // activate it
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * this->normals.size(), this->normals.data(),
								 GL_STATIC_DRAW); /// Create a buffer on GPU memory and copy data
		glVertexAttribPointer( // color
						1,
						3,
						GL_FLOAT,
						GL_FALSE,
						0, // gab between two vertices
						nullptr
		);
		glEnableVertexAttribArray(1); /// 1: attribute number
	}

	if (!this->uvs.empty()) {
		glGenBuffers(1, &this->_vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * this->uvs.size(), this->uvs.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(
						2,
						2,
						GL_FLOAT,
						GL_FALSE,
						0,
						nullptr
		);
		glEnableVertexAttribArray(2);
	}

	if (!this->triangles.empty()) {
		glGenBuffers(1, &this->_vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriangleIndex) * this->triangles.size(), this->triangles.data(),
								 GL_STATIC_DRAW);
	}

	glBindVertexArray(0); /// unbinding
}