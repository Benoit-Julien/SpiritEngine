#include <string>

#include "ShadersFileLoader.hpp"
#include "Scene.hpp"

void ShaderFileLoader::FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory) {
	assert(document.HasMember("shaders"));
	assert(document["shaders"].IsObject());

	assert(document["shaders"].HasMember("vertexes"));
	assert(document["shaders"]["vertexes"].IsArray());

	assert(document["shaders"].HasMember("fragments"));
	assert(document["shaders"]["fragments"].IsArray());

	for (auto &vert : document["shaders"]["vertexes"].GetArray()) {
		assert(vert.IsString());
		std::string vertex = std::string(vert.GetString());
		assert(vertex.rfind(".vert") != std::string::npos);

		Scene::LoadShader<VertexShader>(CleanFilePath(vertex, fileDirectory));
	}

	for (auto &frag : document["shaders"]["fragments"].GetArray()) {
		assert(frag.IsString());
		std::string fragment = std::string(frag.GetString());
		assert(fragment.rfind(".frag") != std::string::npos);

		Scene::LoadShader<FragmentShader>(CleanFilePath(fragment, fileDirectory));
	}
}