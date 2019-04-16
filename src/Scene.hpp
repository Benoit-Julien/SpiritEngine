#ifndef COMPUTERGRAPHICS1_SCENE_HPP
#define COMPUTERGRAPHICS1_SCENE_HPP

#include <memory>
#include <unordered_map>
#include <vector>
#include <exception>
#include <chrono>

#include "Singleton.hpp"
#include "Objects/Drawable.hpp"
#include "Objects/Lights/Light.hpp"
#include "Objects/Lights/SpotLight.hpp"
#include "Objects/Mesh.hpp"

#include "FileLoader/MaterialsFileLoader.hpp"
#include "FileLoader/TexturesFileLoader.hpp"
#include "FileLoader/ShadersFileLoader.hpp"

#define MAX_LIGHTS 32

struct DrawInformation {
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraPosition;
};

class Scene : public Singleton<Scene> {
	friend Singleton<Scene>;

 private:
	Scene() = default;
	virtual ~Scene() = default;

 private:
	std::unordered_map<unsigned int, std::shared_ptr<Drawable>> _objects;
	std::unordered_map<std::string, std::shared_ptr<Material>> _materials;
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
	std::unordered_map<unsigned int, std::shared_ptr<Light>> _lights;
	std::unordered_map<std::string, std::shared_ptr<AShader>> _shaders;

	std::vector<std::pair<std::shared_ptr<Drawable>, std::chrono::time_point<std::chrono::system_clock>>> _toDestroy;

 public:
	template<class T, typename... Args>
	static std::shared_ptr<T> CreateObject(const Args& ...args) {
		auto self = Scene::getSingletonPtr();

		auto obj = std::make_shared<T>(args...);
		self->_objects[obj->getObjectID()] = obj;
		return obj;
	}

	static std::shared_ptr<Mesh> CreateMesh(const std::string &name);

	template<typename... Args>
	static std::shared_ptr<Material> CreateMaterial(const std::string &name, const Args& ...args) {
		auto self = Scene::getSingletonPtr();

		auto elem = self->_materials.find(name);
		if (elem != self->_materials.end())
			return elem->second;

		auto mat = std::make_shared<Material>(args...);
		self->_materials[name] = mat;
		return mat;
	}

	template<class T, typename... Args>
	static std::shared_ptr<T> CreateTexture(const std::string &name, const Args& ...args) {
		auto self = Scene::getSingletonPtr();

		auto elem = self->_textures.find(name);
		if (elem != self->_textures.end())
			return std::dynamic_pointer_cast<T>(elem->second);

		auto tex = std::make_shared<T>(args...);
		self->_textures[name] = tex;
		return tex;
	}

	template<class T, typename... Args>
	static std::shared_ptr<T> CreateLight(const Args& ...args) {
		auto self = Scene::getSingletonPtr();

		if (self->_lights.size() == MAX_LIGHTS * 2)
			return nullptr;

		auto light = std::make_shared<T>(args...);
		self->_lights[light->getObjectID()] = light;
		return light;
	}

	template<typename T>
	static std::shared_ptr<T> LoadShader(const std::string &filename) {
		auto self = Scene::getSingletonPtr();
		auto name = GetFileName(filename, false);

		auto elem = self->_shaders.find(name);
		if (elem != self->_shaders.end())
			return std::dynamic_pointer_cast<T>(elem->second);

		auto shader = std::make_shared<T>();
		shader->initFromFile(filename);
		self->_shaders[name] = shader;
		return shader;
	}

	static std::shared_ptr<Drawable> FindObjectByID(const unsigned int &ID);
	static std::shared_ptr<Drawable> FindObjectByType(const ObjectType &type);
	static std::vector<std::shared_ptr<Drawable>> FindObjectsByType(const ObjectType &type);

	static void Destroy(std::shared_ptr<Drawable> object, const float &time = 0);

	static void RemoveMaterial(const std::string &name);
	static std::shared_ptr<Material> FindMaterial(const std::string &name);
	inline static void LoadMaterialFile(const std::string &filename) { MaterialsFileLoader::LoadFile(filename); }

	static void RemoveTexture(const std::string &name);
	static std::shared_ptr<Texture> FindTexture(const std::string &name);
	inline static void LoadTextureFile(const std::string &filename) { TexturesFileLoader::LoadFile(filename); }

	static void RemoveLight(const unsigned int &ID);
	static std::shared_ptr<Light> FindLight(const unsigned int &ID);

	template<typename T>
	static std::shared_ptr<T> FindShader(const std::string &name) {
		auto self = Scene::getSingletonPtr();

		auto elem = self->_shaders.find(name);
		if (elem != self->_shaders.end())
			return std::dynamic_pointer_cast<T>(elem->second);
		return nullptr;
	}
	inline static void LoadShaderFile(const std::string &filename) { ShaderFileLoader::LoadFile(filename); }

	static void BeforeDrawing();
	static void PhysicalUpdate();
	static void Draw(const DrawInformation &info);
	//static void DrawObjectsList();

 private:
	void recursiveDraw(std::shared_ptr<Drawable> obj, const DrawInformation &info, const glm::mat4 &model);
	void setLightsToShader(std::shared_ptr<Material> material, const DrawInformation &info);
};

#endif /* !COMPUTERGRAPHICS1_SCENE_HPP */
