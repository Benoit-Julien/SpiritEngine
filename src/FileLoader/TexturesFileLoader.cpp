#include "TexturesFileLoader.hpp"
#include "Scene.hpp"
#include "Texture/Texture2D.hpp"
#include "Texture/SkyBoxTexture.hpp"

static std::unordered_map<std::string, Texture::MinFilterEnum> MinFilterDict = {
				{"NEAREST",                Texture::MinFilterEnum::NEAREST},
				{"LINEAR",                 Texture::MinFilterEnum::LINEAR},
				{"NEAREST_MIPMAP_NEAREST", Texture::MinFilterEnum::NEAREST_MIPMAP_NEAREST},
				{"LINEAR_MIPMAP_NEAREST",  Texture::MinFilterEnum::LINEAR_MIPMAP_NEAREST},
				{"NEAREST_MIPMAP_LINEAR",  Texture::MinFilterEnum::NEAREST_MIPMAP_LINEAR},
				{"LINEAR_MIPMAP_LINEAR",   Texture::MinFilterEnum::LINEAR_MIPMAP_LINEAR}
};

static std::unordered_map<std::string, Texture::MagFilterEnum> MagFilterDict = {
				{"NEAREST", Texture::MagFilterEnum::NEAREST},
				{"LINEAR",  Texture::MagFilterEnum::LINEAR}
};

static std::unordered_map<std::string, Texture::WrapEnum> SWrapDict = {
				{"CLAMP_TO_EDGE",   Texture::WrapEnum::CLAMP_TO_EDGE},
				{"MIRRORED_REPEAT", Texture::WrapEnum::MIRRORED_REPEAT},
				{"REPEAT",          Texture::WrapEnum::REPEAT},
};
static decltype(SWrapDict) &TWrapDict = SWrapDict;
static decltype(SWrapDict) &RWrapDict = SWrapDict;
static std::unordered_map<std::string, Texture::ColorFormatEnum> ColorFormatDict = {
				{"RED",             Texture::ColorFormatEnum::RED},
				{"RG",              Texture::ColorFormatEnum::RG},
				{"RGB",             Texture::ColorFormatEnum::RGB},
				{"RGBA",            Texture::ColorFormatEnum::RGBA},
				{"DEPTH_COMPONENT", Texture::ColorFormatEnum::DEPTH_COMPONENT},
				{"DEPTH_STENCIL",   Texture::ColorFormatEnum::DEPTH_STENCIL}
};

void TexturesFileLoader::FileTreatment(const rapidjson::Document &document, const std::string &fileDirectory) {
	assert(document.HasMember("textures") && document["textures"].IsArray());
	for (auto &texture : document["textures"].GetArray()) {
		assert(texture.IsObject());
		assert(texture.HasMember("name") && texture["name"].IsString());
		assert(texture.HasMember("type") && texture["type"].IsString());

		std::string type = texture["type"].GetString();

		if (type == "texture2d") {
			assert(texture.HasMember("image") && texture["image"].IsString());

			auto tex = Scene::CreateTexture<Texture2D>(texture["name"].GetString());

			ATTRIBUTE_CHECKER(MinFilter, "minFilter")
			ATTRIBUTE_CHECKER(MagFilter, "magFilter")
			ATTRIBUTE_CHECKER(SWrap, "sWrap")
			ATTRIBUTE_CHECKER(TWrap, "tWrap")
			ATTRIBUTE_CHECKER(ColorFormat, "colorFormat")
			tex->initFromFile(CleanFilePath(texture["image"].GetString(), fileDirectory));
		}
		else if (type == "skybox") {
			assert(texture.HasMember("images") && texture["images"].IsObject());

			std::unordered_map<SkyBoxTexture::Face, std::string> cubeMap;

			GET_SKYBOX_TEXTURE("left", SkyBoxTexture::Face::LEFT)
			GET_SKYBOX_TEXTURE("right", SkyBoxTexture::Face::RIGHT)
			GET_SKYBOX_TEXTURE("top", SkyBoxTexture::Face::TOP)
			GET_SKYBOX_TEXTURE("down", SkyBoxTexture::Face::DOWN)
			GET_SKYBOX_TEXTURE("front", SkyBoxTexture::Face::FRONT)
			GET_SKYBOX_TEXTURE("back", SkyBoxTexture::Face::BACK)

			auto tex = Scene::CreateTexture<SkyBoxTexture>(texture["name"].GetString());
			tex->initFromCubeMap(cubeMap);
		}
	}
}