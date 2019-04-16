#ifndef COMPUTERGRAPHICS1_MOVABLE_HPP
#define COMPUTERGRAPHICS1_MOVABLE_HPP

#include "Object.hpp"

class Movable : public Object {
 private:
	glm::vec3 _position;
	glm::mat4 _rotation;
	glm::vec3 _scale;

	glm::mat4 _modelMatrix;

	bool _translateLocked;
	bool _rotateLocked;
	bool _scaleLocked;

 public:
	std::shared_ptr<Movable> Parent;
	std::vector<std::shared_ptr<Movable>> Children;

 public:
	Movable(const ObjectType &type);
	virtual ~Movable();

	Movable(const Movable &movable);
	Movable &operator=(const Movable &movable);

	inline const glm::vec3 &getPosition() const { return this->_position; }
	inline const glm::mat4 &getRotation() const { return this->_rotation; }
	inline const glm::vec3 &getScale() const { return this->_scale; }

	inline const glm::mat4 &getModelMatrix() const { return this->_modelMatrix; }

	void Translate(const glm::vec3 &vec);
	void Rotate(const float &degree, const glm::vec3 &axis);
	void Rotate(const glm::mat4 &mat);
	void Scale(const glm::vec3 &vec);

	inline void LockTranslate() { this->_translateLocked = true; }
	inline void LockRotate() { this->_rotateLocked = true; }
	inline void LockScale() { this->_scaleLocked = true; }

	inline void UnlockTranslate() { this->_translateLocked = false; }
	inline void UnlockRotate() { this->_rotateLocked = false; }
	inline void UnlockScale() { this->_scaleLocked = false; }

	inline bool IsTranlateLocked() const { return this->_translateLocked; }
	inline bool IsRotateLocked() const { return this->_rotateLocked; }
	inline bool IsScaleLocked() const { return this->_scaleLocked; }

	glm::vec3 ToWorldPosition() const;
	glm::mat4 ToWorldRotation() const;
	glm::vec3 ToWorldScale() const;

	void SetParent(std::shared_ptr<Movable> parent);
	void AddChildren(std::shared_ptr<Movable> child);
	void RemoveChildren(std::shared_ptr<Movable> child);
	void RemoveChildren(const unsigned int &ID);

 private:
	glm::mat4 getLocalTransformation() const;
};


#endif /* !COMPUTERGRAPHICS1_MOVABLE_HPP */
