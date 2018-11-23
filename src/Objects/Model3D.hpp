#ifndef COMPUTERGRAPHICS1_MODEL3D_HPP
#define COMPUTERGRAPHICS1_MODEL3D_HPP

#include <string>

#include "Drawable.hpp"
#include "TriangleObject.hpp"

class Model3D : public Drawable {
	std::string _filePath;
	std::vector<std::shared_ptr<TriangleObject>> _childObjects;

	DrawMode _drawMode;

 public:
	Model3D(const std::string &filePath);
	virtual ~Model3D();

	Model3D(const Model3D &model);
	Model3D &operator=(const Model3D &model);

	virtual void Draw();

	void setDrawMode(const DrawMode &mode);

 private:
	void setup();
};

#endif /* !COMPUTERGRAPHICS1_MODEL3D_HPP */
