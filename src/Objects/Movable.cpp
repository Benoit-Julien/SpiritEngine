#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Movable.hpp"
#include "Scene.hpp"

Movable::Movable(const ObjectType &type) : Object(type) {
	this->_position = glm::vec3(0, 0, 0);
	this->_rotation = glm::mat4(1.0f);
	this->_scale = glm::vec3(1, 1, 1);
	this->_modelMatrix = glm::mat4(1.0f);

	this->_translateLocked = false;
	this->_rotateLocked = false;
	this->_scaleLocked = false;

	this->Parent = nullptr;
}

Movable::~Movable() {}

Movable::Movable(const Movable &movable)
				: Object(movable),
					_position(movable._position),
					_rotation(movable._rotation),
					_scale(movable._scale),
					_modelMatrix(movable._modelMatrix),
					_translateLocked(movable._translateLocked),
					_rotateLocked(movable._rotateLocked),
					_scaleLocked(movable._scaleLocked),
					Parent(movable.Parent),
					Children(movable.Children) {
}

Movable &Movable::operator=(const Movable &movable) {
	this->_position = movable._position;
	this->_rotation = movable._rotation;
	this->_scale = movable._scale;
	this->_modelMatrix = movable._modelMatrix;
	this->_translateLocked = movable._translateLocked;
	this->_rotateLocked = movable._rotateLocked;
	this->_scaleLocked = movable._scaleLocked;
	this->Parent = movable.Parent;
	this->Children = movable.Children;
	return *this;
}

void Movable::Translate(const glm::vec3 &vec) {
	if (this->_translateLocked)
		return;

	this->_position += vec;
	this->_modelMatrix *= glm::translate(glm::mat4(1.0f), vec);
}

void Movable::Rotate(const float &degree, const glm::vec3 &axis) {
	if (this->_rotateLocked)
		return;

	glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(degree), axis);
	this->Rotate(rot);
}

void Movable::Rotate(const glm::mat4 &mat) {
	if (this->_rotateLocked)
		return;

	this->_rotation = this->_rotation * mat;
	this->_modelMatrix = this->_modelMatrix * mat;
}

void Movable::Scale(const glm::vec3 &vec) {
	if (this->_scaleLocked)
		return;

	this->_scale *= vec;
	this->_modelMatrix *= glm::scale(glm::mat4(1.0f), vec);
}

glm::mat4 Movable::getLocalTransformation() const {
	glm::mat4 mat = this->getModelMatrix();

	if (this->Parent)
		mat = this->Parent->getLocalTransformation() * mat;
	return mat;
}

glm::vec3 Movable::ToWorldPosition() const {
	return this->getLocalTransformation()[3];
}

glm::mat4 Movable::ToWorldRotation() const {
	glm::mat4 mat = this->getLocalTransformation();
	glm::vec3 scale = this->ToWorldScale();

	return mat * glm::mat4{
					glm::vec4(1 / scale.x, 0, 0, 0),
					glm::vec4(0, 1 / scale.y, 0, 0),
					glm::vec4(0, 0, 1 / scale.z, 0),
					glm::vec4(0, 0, 0, 1)
	};
}

glm::vec3 Movable::ToWorldScale() const {
	glm::mat4 mat = this->getLocalTransformation();

	return glm::vec3(glm::length(mat[0]), glm::length(mat[1]), glm::length(mat[2]));
}

void Movable::SetParent(std::shared_ptr<Movable> parent) {
	if (parent == nullptr) {
		this->Parent->RemoveChildren(this->getObjectID());
		this->Parent = nullptr;
	}
	else {
		this->Parent = parent;
		this->Parent->AddChildren(Scene::FindObjectByID(this->getObjectID()));
	}
}

void Movable::AddChildren(std::shared_ptr<Movable> child) {
	if (std::find(this->Children.begin(), this->Children.end(), child) != this->Children.end())
		return;
	this->Children.push_back(child);
}

void Movable::RemoveChildren(std::shared_ptr<Movable> child) {
	this->RemoveChildren(child->getObjectID());
}

void Movable::RemoveChildren(const unsigned int &ID) {
	auto elem = std::find(this->Children.begin(), this->Children.end(), ID);
	if (elem != this->Children.end())
		return;
	this->Children.erase(elem);
}