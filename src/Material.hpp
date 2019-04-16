#ifndef COMPUTERGRAPHICS1_MATERIAL_HPP
#define COMPUTERGRAPHICS1_MATERIAL_HPP

#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include "Shader/ShaderProgram.hpp"
#include "Texture/Texture.hpp"

#define FIND_TEXTURE \
  auto t = std::find_if(this->textures.begin(), this->textures.end(), [texture](decltype(texture) a) { \
    return a.get() == texture.get(); \
  });

#define FIND_NORMAL_MAP \
  auto n = std::find_if(this->normalMaps.begin(), this->normalMaps.end(), [normalMap](decltype(normalMap) a) { \
    return a.get() == normalMap.get(); \
  });

#define MAX_TEXTURES GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1

class Light;

struct ShaderVariables {
	const glm::mat4 &modelMatrix;
	const glm::mat3 &normalMatrix;
	const glm::mat4 &viewMatrix;
	const glm::mat4 &projectionMatrix;
	const glm::vec3 &cameraPosition;

	const unsigned int lightNumber;
	const std::unordered_map<unsigned int, std::shared_ptr<Light>> &lights;

	ShaderVariables(const glm::mat4 &m, const glm::mat3 &n, const glm::mat4 &v,
									const glm::mat4 &p, const glm::vec3 &camPos,
									const std::unordered_map<unsigned int, std::shared_ptr<Light>> &l)
					: modelMatrix(m), normalMatrix(n), viewMatrix(v),
						projectionMatrix(p), lightNumber(l.size()),
						cameraPosition(camPos), lights(l) {}
};

class Material {
 public:
	std::shared_ptr<ShaderProgram> shader;
	std::vector<std::shared_ptr<Texture>> textures;
	std::shared_ptr<Texture> normalMap;

	glm::vec3 Diffuse;
	glm::vec3 Ambient;
	glm::vec3 Specular;
	float shiness;

 public:
	Material();
	Material(const std::string &vertexShader, const std::string &fragmentShader);
	Material(std::shared_ptr<ShaderProgram> shader);
	~Material();

	void use();
	void disable();

	void AddTexture(std::shared_ptr<Texture> texture);
	void RemoveTexture(std::shared_ptr<Texture> texture);

 private:
	void initialize();
};


#endif /* !COMPUTERGRAPHICS1_MATERIAL_HPP */
