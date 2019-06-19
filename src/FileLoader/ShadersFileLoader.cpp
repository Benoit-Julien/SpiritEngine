#include "ShadersFileLoader.hpp"
#include "Scene.hpp"
#include "PostProcessing.hpp"

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

	if (document.HasMember("post-processing")) {
		assert(document["post-processing"].IsArray());
		for (auto &pp : document["post-processing"].GetArray()) {
			assert(pp.IsObject());

			assert(pp.HasMember("name") && pp["name"].IsString());
			assert(pp.HasMember("vertex") && pp["vertex"].IsString());
			assert(pp.HasMember("fragment") && pp["fragment"].IsString());

			std::string name = pp["name"].GetString();
			std::string vertex = pp["vertex"].GetString();
			std::string fragment = pp["fragment"].GetString();

			assert(vertex.rfind(".vert") != std::string::npos);
			assert(fragment.rfind(".frag") != std::string::npos);

			PostProcessing::AddPostProcessing(name, CleanFilePath(vertex, fileDirectory), CleanFilePath(fragment, fileDirectory));
		}
	}
}