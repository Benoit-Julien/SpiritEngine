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

#include "FileLoader/MaterialsFileLoader.hpp"
#include "FileLoader/TexturesFileLoader.hpp"

#define MAX_LIGHTS 32

struct DrawInformation {
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
};

class Scene : public Singleton<Scene> {
	friend Singleton<Scene>;

	std::unordered_map<unsigned int, std::shared_ptr<Drawable>> _objects;
	std::unordered_map<std::string, std::shared_ptr<Material>> _materials;
	std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
	std::unordered_map<unsigned int, std::shared_ptr<Light>> _lights;

	std::vector<std::pair<std::shared_ptr<Drawable>, std::chrono::time_point<std::chrono::system_clock>>> _toDestroy;

	Scene() = default;
	virtual ~Scene() = default;

 public:
	template<class T, typename... Args>
	static std::shared_ptr<T> CreateObject(const Args& ...args) {
		auto self = Scene::getSingletonPtr();

		auto obj = std::make_shared<T>(args...);
		self->_objects[obj->getObjectID()] = obj;
		return obj;
	}

	template<typename... Args>
	static std::shared_ptr<Material> CreateMaterial(const std::string &name, const Args& ...args) {
		auto self = Scene::getSingletonPtr();

		if (self->_materials.find(name) != self->_materials.end())
			throw std::logic_error("Cannot create a material named " + name + " because an other with the same name already exist.");

		auto mat = std::make_shared<Material>(args...);
		self->_materials[name] = mat;
		return mat;
	}

	static std::shared_ptr<Texture> CreateTexture(const std::string &name) {
		auto self = Scene::getSingletonPtr();

		if (self->_textures.find(name) != self->_textures.end())
			throw std::logic_error("Cannot create a texture named " + name + " because an other with the same name already exist.");

		auto tex = std::make_shared<Texture>();
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

	static void BeforeDrawing();
	static void PhysicalUpdate();
	static void Draw(const DrawInformation &info);

 private:
	void recursiveDraw(std::shared_ptr<Drawable> obj, const DrawInformation &info, const glm::mat4 &model);
	void setLightsToShader(std::shared_ptr<Material> material, const DrawInformation &info);
};

#endif /* !COMPUTERGRAPHICS1_SCENE_HPP */
