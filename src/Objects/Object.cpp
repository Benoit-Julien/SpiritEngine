#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <memory>

#include "Object.hpp"
#include "IDGenerator.hpp"
#include "Scene.hpp"

Object::Object(const ObjectType &type) : _type(type) {
	this->_id = IDGenerator::getSingleton().generateUniqueId();
}

Object::~Object() {
	IDGenerator::getSingleton().removeUniqueId(this->_id);
}

Object::Object(const Object &object) : _type(object._type) {
	this->_id = IDGenerator::getSingleton().generateUniqueId();
}

Object& Object::operator=(const Object &object) {
	this->_type = object._type;
	this->_id = IDGenerator::getSingleton().generateUniqueId();
	return *this;
}