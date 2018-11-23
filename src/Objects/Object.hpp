#ifndef COMPUTERGRAPHICS1_OBJECT_HPP
#define COMPUTERGRAPHICS1_OBJECT_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <vector>
#include <memory>

#include "ObjectTypes.hpp"
#include "Material.hpp"

class Object {
 private:
	unsigned int _id;
	ObjectType _type;

 public:
	Object(const ObjectType &type);
	virtual ~Object();

	Object(const Object &object);
	Object(Object &&) = delete;
	Object &operator=(const Object &object);
	Object &operator=(Object &&) = delete;

	bool operator==(const Object &obj) { return this->getObjectID() == obj.getObjectID(); }
	bool operator==(const unsigned int &ID) { return this->getObjectID() == ID; }
	friend bool operator==(std::shared_ptr<Object> a, std::shared_ptr<Object> b) { return (*a == *b); }
	friend bool operator==(std::shared_ptr<Object> a, const unsigned int &ID) { return (*a == ID); }

	bool operator!=(const Object &obj) { return !(*this == obj); }
	bool operator!=(const unsigned int &ID) { return !(*this == ID); }
	friend bool operator!=(std::shared_ptr<Object> a, std::shared_ptr<Object> b) { return !(a == b); }
	friend bool operator!=(std::shared_ptr<Object> a, const unsigned int &ID) { return !(a == ID); }

	inline const unsigned int &getObjectID() const { return this->_id; }
	inline const ObjectType &getType() const { return this->_type; }
};

#endif /* !COMPUTERGRAPHICS1_OBJECT_HPP */
