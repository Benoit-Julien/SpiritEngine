#include "Drawable.hpp"

Drawable::Drawable(const ObjectType &type) : Movable(type) {
	this->_enabled = true;
}

Drawable::~Drawable() {}

Drawable::Drawable(const Drawable &drawable)
				: Movable(drawable),
					_enabled(drawable._enabled),
					material(drawable.material) {

}

Drawable &Drawable::operator=(const Drawable &drawable) {
	this->_enabled = drawable._enabled;
	this->material = drawable.material;
	return *this;
}