#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "Movable.hpp"
#include "Scene.hpp"

Movable::Movable(const ObjectType &type) : Object(type) {
	this->_position = glm::vec3(0, 0, 0);
	this->_rotation = glm::identity<glm::quat>();
	this->_scale = glm::vec3(1, 1, 1);

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
	this->_translateLocked = movable._translateLocked;
	this->_rotateLocked = movable._rotateLocked;
	this->_scaleLocked = movable._scaleLocked;
	this->Parent = movable.Parent;
	this->Children = movable.Children;
	return *this;
}

void Movable::Translate(const glm::vec3 &vec) {
	if (this->_translateLocked) return;

	this->_position += vec;
}

/*void Movable::Rotate(const float &degree, const glm::vec3 &axis) {
	if (this->_rotateLocked) return;

	this->_rotation = this->_rotation * glm::angleAxis(glm::radians(degree), axis);
}*/

void Movable::Scale(const glm::vec3 &vec) {
	if (this->_scaleLocked) return;

	this->_scale *= vec;
}

void Movable::SetPosition(const glm::vec3 &pos) {
	if (this->_translateLocked) return;

	this->_position = pos;
}

void Movable::SetRotation(const glm::quat &rot) {
	if (this->_rotateLocked) return;

	this->_rotation = rot;
}

void Movable::SetEulerAngles(const glm::vec3 &euler) {
	if (this->_rotateLocked) return;

	this->_rotation = glm::quat(euler);
}

void Movable::SetScale(const glm::vec3 &scale) {
	if (this->_scaleLocked) return;
}

glm::mat4 Movable::GetLocalModelMatrix() const {
	auto trans = glm::translate(glm::mat4(1.0f), this->_position);
	auto rot = glm::toMat4(this->_rotation);
	auto scale = glm::scale(glm::mat4(1.0f), this->_scale);

	return scale * rot * trans;
}

glm::mat4 Movable::GetWorldModelMatrix() const {
	auto mat = this->GetLocalModelMatrix();

	if (this->Parent != nullptr)
		mat = this->Parent->GetWorldModelMatrix() * mat;
	return mat;
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