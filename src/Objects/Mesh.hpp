#ifndef COMPUTERGRAPHICS1_MODEL3D_HPP
#define COMPUTERGRAPHICS1_MODEL3D_HPP

#include <string>

#include "Drawable.hpp"
#include "TriangleObject.hpp"

class Mesh : public Drawable {
	std::string _filePath;
	std::vector<std::shared_ptr<TriangleObject>> _childObjects;
	std::vector<std::shared_ptr<Material>> _materials;

	DrawMode _drawMode;

 public:
	Mesh(const std::string &filePath);
	virtual ~Mesh();

	Mesh(const Mesh &model);
	Mesh &operator=(const Mesh &model);

	virtual void Draw(const ShaderVariables &variables);

	void setDrawMode(const DrawMode &mode);
	inline std::shared_ptr<Material> GetMaterial(const std::size_t &index) { return this->_materials[index]; }

 private:
	void setup();
};

#endif /* !COMPUTERGRAPHICS1_MODEL3D_HPP */
