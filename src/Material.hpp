#ifndef COMPUTERGRAPHICS1_MATERIAL_HPP
#define COMPUTERGRAPHICS1_MATERIAL_HPP

#include <memory>
#include <string>
#include <glm/vec3.hpp>

#include "Shader.hpp"

class Material {
 public:
	std::shared_ptr<Shader> shader;

	glm::vec3 Diffuse;
	glm::vec3 Ambient;
	glm::vec3 Specular;
	float phong;

 public:
	Material();
	Material(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);
	~Material();

	void use();
	void disable();

 private:
	void initialize();
};


#endif /* !COMPUTERGRAPHICS1_MATERIAL_HPP */
