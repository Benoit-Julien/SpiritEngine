#ifndef SPIRITENGINE_SHADERLOADER_HPP
#define SPIRITENGINE_SHADERLOADER_HPP

#include <unordered_map>
#include <string>
#include <memory>

#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include "Singleton.hpp"

class ShaderLoader : private Singleton<ShaderLoader> {
	friend Singleton<ShaderLoader>;

	std::unordered_map<std::string, std::shared_ptr<AShader>> _shaderList;

	ShaderLoader() = default;
	virtual ~ShaderLoader() = default;

 public:
	template<typename T>
	static std::shared_ptr<T> LoadShader(const std::string &filename) {
		auto self = ShaderLoader::getSingletonPtr();

		auto elem = self->_shaderList.find(filename);
		if (elem != self->_shaderList.end())
			return std::dynamic_pointer_cast<T>(elem->second);

		auto shader = std::make_shared<T>();
		shader->initFromFile(filename);
		self->_shaderList[filename] = shader;
		return shader;
	}
};

#endif /* !SPIRITENGINE_SHADERLOADER_HPP */
