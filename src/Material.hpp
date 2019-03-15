#ifndef COMPUTERGRAPHICS1_MATERIAL_HPP
#define COMPUTERGRAPHICS1_MATERIAL_HPP

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include "Shader/ShaderProgram.hpp"
#include "Texture.hpp"

class Light;

struct ShaderVariables {
	const glm::mat4 &modelMatrix;
	const glm::mat3 &normalMatrix;
	const glm::mat4 &viewMatrix;
	const glm::mat4 &projectionMatrix;

	const unsigned int lightNumber;
	const std::unordered_map<unsigned int, std::shared_ptr<Light>> &lights;

	ShaderVariables(const glm::mat4 &m, const glm::mat3 &n, const glm::mat4 &v, const glm::mat4 &p,
									const std::unordered_map<unsigned int, std::shared_ptr<Light>> &l)
					: modelMatrix(m), normalMatrix(n), viewMatrix(v),
						projectionMatrix(p), lightNumber(l.size()), lights(l) {}
};

class Material {
 public:
	std::shared_ptr<ShaderProgram> shader;
	std::shared_ptr<Texture> texture;

	glm::vec3 Diffuse;
	glm::vec3 Ambient;
	glm::vec3 Specular;
	float shiness;

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
