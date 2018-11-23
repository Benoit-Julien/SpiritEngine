#ifndef COMPUTERGRAPHICS1_MODELLOADER_HPP
#define COMPUTERGRAPHICS1_MODELLOADER_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "Singleton.hpp"
#include "Objects/Model3D.hpp"

class ModelLoader : private Singleton<ModelLoader> {
	friend Singleton<ModelLoader>;

	std::unordered_map<std::string, std::shared_ptr<Model3D>> _modelList;

	ModelLoader() = default;
	virtual ~ModelLoader() = default;

 public:
	static std::shared_ptr<Model3D> LoadModel(const std::string &fileName) {
		auto self = ModelLoader::getSingletonPtr();

		auto elem = self->_modelList.find(fileName);
		if (elem != self->_modelList.end())
			return std::make_shared<Model3D>(*elem->second);

		auto model = std::make_shared<Model3D>(fileName);
		self->_modelList[fileName] = model;
		return std::make_shared<Model3D>(*model);
	}
};


#endif /* !COMPUTERGRAPHICS1_MODELLOADER_HPP */
