#ifndef COMPUTERGRAPHICS1_MODELLOADER_HPP
#define COMPUTERGRAPHICS1_MODELLOADER_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "Singleton.hpp"
#include "Objects/Mesh.hpp"

class ModelLoader : private Singleton<ModelLoader> {
	friend Singleton<ModelLoader>;

	std::unordered_map<std::string, std::shared_ptr<Mesh>> _modelList;

	ModelLoader() = default;
	virtual ~ModelLoader() = default;

 public:
	static std::shared_ptr<Mesh> LoadModel(const std::string &fileName) {
		auto self = ModelLoader::getSingletonPtr();

		auto elem = self->_modelList.find(fileName);
		if (elem != self->_modelList.end())
			return std::make_shared<Mesh>(*elem->second);

		auto model = std::make_shared<Mesh>(fileName);
		self->_modelList[fileName] = model;
		return std::make_shared<Mesh>(*model);
	}
};


#endif /* !COMPUTERGRAPHICS1_MODELLOADER_HPP */
