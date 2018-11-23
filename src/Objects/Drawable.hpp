#ifndef COMPUTERGRAPHICS1_DRAWABLE_HPP
#define COMPUTERGRAPHICS1_DRAWABLE_HPP

#include "Movable.hpp"
#include "Material.hpp"

class Drawable : public Movable {
	bool _enabled;

 public:
	std::shared_ptr<Material> material;

 public:
	Drawable(const ObjectType &type);
	virtual ~Drawable();

	Drawable(const Drawable &drawable);
	Drawable &operator=(const Drawable &drawable);

	virtual void Draw() = 0;

	inline void SetEnable(const bool &enable) { this->_enabled = enable; }
	inline const bool &Enabled() const { return this->_enabled; }
};

#endif /* !COMPUTERGRAPHICS1_DRAWABLE_HPP */
