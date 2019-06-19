#ifndef COMPUTERGRAPHICS1_TRIANGLEOBJECT_HPP
#define COMPUTERGRAPHICS1_TRIANGLEOBJECT_HPP

#include <vector>

#include "Drawable.hpp"

enum class DrawMode {
	Shape,
	WireFrame
};

class TriangleObject : public Drawable {
 private:
	enum BUFFERS {
		VERTEX_BUFFER,
		TEXCOORD_BUFFER,
		NORMAL_BUFFER,
		TANGENT_BUFFER,
		BIT_TANGENT_BUFFER,
		INDEX_BUFFER
	};

 public:
	struct TriangleIndex {
		GLuint a, b, c;

		TriangleIndex() = default;

		TriangleIndex(GLuint i1, GLuint i2, GLuint i3) : a(i1), b(i2), c(i3) {}
	};

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitTangents;
	std::vector<TriangleIndex> triangles;

	bool useOwnMaterial;

 private:
	DrawMode _drawMode;

	GLuint _vao; /// id or handler
	GLuint _vbo[6];

 public:
	TriangleObject();
	TriangleObject(const ObjectType &type);
	virtual ~TriangleObject();

	TriangleObject(const TriangleObject &object);
	TriangleObject &operator=(const TriangleObject &object);

	virtual void Draw(const ShaderVariables &variables);

	void setup();

	inline void setDrawMode(const DrawMode &mode) { this->_drawMode = mode; }

	inline const DrawMode &getDrawMode() const { return this->_drawMode; }

 protected:
	virtual void beforeDraw() {}
	virtual void afterDraw() {}
};

#endif /* !COMPUTERGRAPHICS1_TRIANGLEOBJECT_HPP */
