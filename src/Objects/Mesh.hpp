#ifndef COMPUTERGRAPHICS1_MODEL3D_HPP
#define COMPUTERGRAPHICS1_MODEL3D_HPP

#include <string>
#include <unordered_map>
#include <iostream>

#include "Drawable.hpp"
#include "TriangleObject.hpp"

class Mesh : public Drawable {
	std::string _filePath;
	std::vector<std::shared_ptr<TriangleObject>> _childObjects;
	std::vector<std::shared_ptr<Material>> _materials;
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;

	DrawMode _drawMode;

	std::shared_ptr<ShaderProgram> _shader;

 public:
	explicit Mesh();
	virtual ~Mesh();

	Mesh(const Mesh &model);
	Mesh &operator=(const Mesh &model);

	virtual void Draw(const ShaderVariables &) { }

	void setDrawMode(const DrawMode &mode);
	inline std::shared_ptr<Material> GetMaterial(const std::size_t &index) { return this->_materials[index]; }

	void SetShader(const std::string &vertexShader, const std::string &fragmentShader);
	inline std::shared_ptr<ShaderProgram> GetShader() const { return this->_shader; };

	void LoadMesh(const std::string &filePath);

	virtual void SetCullFaceOption(const GLenum &cullface);

	virtual void EnableCulling();
	virtual void DisableCulling();

	template<typename T>
	inline void SetCustomUniform(const std::string &varName, const T &value) {
		for (auto &obj : this->_childObjects) obj->SetCustomUniform(varName, value);
	}
	void RemoveCustomUniform(const std::string &varName) {
		for (auto &obj : this->_childObjects) obj->RemoveCustomUniform(varName);
	}

 private:
	void setup();
};

#endif /* !COMPUTERGRAPHICS1_MODEL3D_HPP */
