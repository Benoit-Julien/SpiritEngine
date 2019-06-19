#ifndef COMPUTERGRAPHICS1_MOVABLE_HPP
#define COMPUTERGRAPHICS1_MOVABLE_HPP

#include <glm/matrix.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Object.hpp"

class Movable : public Object {
 private:
	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

	bool _translateLocked;
	bool _rotateLocked;
	bool _scaleLocked;

 public:
	std::shared_ptr<Movable> Parent;
	std::vector<std::shared_ptr<Movable>> Children;

 public:
	explicit Movable(const ObjectType &type);
	virtual ~Movable();

	Movable(const Movable &movable);
	Movable &operator=(const Movable &movable);

	inline const glm::vec3 &GetPosition() const { return this->_position; }
	inline const glm::quat &GetRotation() const { return this->_rotation; }
	inline const glm::vec3 &GetScale() const { return this->_scale; }

	inline glm::vec3 &GetPosition() { return this->_position; }
	inline glm::quat &GetRotation() { return this->_rotation; }
	inline glm::vec3 &GetScale() { return this->_scale; }

	void Translate(const glm::vec3 &vec);
	//void Rotate(const float &degree, const glm::vec3 &axis);
	void Scale(const glm::vec3 &vec);

	void SetPosition(const glm::vec3 &pos);
	void SetRotation(const glm::quat &rot);
	void SetEulerAngles(const glm::vec3 &euler);
	void SetScale(const glm::vec3 &scale);

	inline void LockTranslate() { this->_translateLocked = true; }
	inline void LockRotate() { this->_rotateLocked = true; }
	inline void LockScale() { this->_scaleLocked = true; }

	inline void UnlockTranslate() { this->_translateLocked = false; }
	inline void UnlockRotate() { this->_rotateLocked = false; }
	inline void UnlockScale() { this->_scaleLocked = false; }

	inline bool IsTranslateLocked() const { return this->_translateLocked; }
	inline bool IsRotateLocked() const { return this->_rotateLocked; }
	inline bool IsScaleLocked() const { return this->_scaleLocked; }

	glm::mat4 GetLocalModelMatrix() const;
	glm::mat4 GetWorldModelMatrix() const;
	//glm::vec3 ToWorldPosition() const;
	//glm::quat ToWorldRotation() const;
	//glm::vec3 ToWorldScale() const;

	void SetParent(std::shared_ptr<Movable> parent);
	void AddChildren(std::shared_ptr<Movable> child);
	void RemoveChildren(std::shared_ptr<Movable> child);
	void RemoveChildren(const unsigned int &ID);

 //private:
	//glm::mat4 getLocalTransformation() const;
};


#endif /* !COMPUTERGRAPHICS1_MOVABLE_HPP */
