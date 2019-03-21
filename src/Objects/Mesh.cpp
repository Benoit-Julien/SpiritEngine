#include <assimp/scene.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include <exception>

#include "Mesh.hpp"
#include "Objects/Lights/Light.hpp"
#include "utils.hpp"
#include "Scene.hpp"

Mesh::Mesh()
				: Drawable(ObjectType::Model3D),
					_drawMode(DrawMode::Shape) {
}

Mesh::~Mesh() {}

Mesh::Mesh(const Mesh &model)
				: Drawable(model),
					_filePath(model._filePath),
					_drawMode(model._drawMode),
					_childObjects(model._childObjects.size()) {
	for (auto i = 0; i < model._childObjects.size(); i++)
		this->_childObjects[i] = std::make_shared<TriangleObject>(*model._childObjects[i]);
}

Mesh &Mesh::operator=(const Mesh &model) {
	this->_filePath = model._filePath;
	this->_drawMode = model._drawMode;
	this->_childObjects = std::vector<std::shared_ptr<TriangleObject>>(model._childObjects.size());
	for (auto i = 0; i < model._childObjects.size(); i++)
		this->_childObjects[i] = std::make_shared<TriangleObject>(*model._childObjects[i]);
	return *this;
}

void Mesh::setDrawMode(const DrawMode &mode) {
	_drawMode = mode;
	for (auto &obj : this->_childObjects)
		obj->setDrawMode(mode);
}

void Mesh::SetCullFaceOption(const GLenum &cullface) {
	for (auto &obj : this->_childObjects) obj->SetCullFaceOption(cullface);
	CullFace::SetCullFaceOption(cullface);
}

void Mesh::EnableCulling() {
	for (auto &obj : this->_childObjects) obj->EnableCulling();
	CullFace::EnableCulling();
}

void Mesh::DisableCulling() {
	for (auto &obj : this->_childObjects) obj->DisableCulling();
	CullFace::DisableCulling();
}

void Mesh::SetShader(const std::string &vertexShader, const std::string &fragmentShader) {
	this->_shader = std::make_shared<ShaderProgram>();
	auto vertex = Scene::FindShader<VertexShader>(vertexShader);
	auto fragment = Scene::FindShader<FragmentShader>(fragmentShader);
	this->_shader->initFromShader(vertex, fragment);
}

void Mesh::LoadMesh(const std::string &filePath) {
	this->_filePath = CleanFilePath(filePath);
	this->setup();
}

void Mesh::setup() {
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene *scene = importer.ReadFile(this->_filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes);
	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		throw std::logic_error("ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));

	auto fileDirectory = GetFilePath(this->_filePath);
	auto filename = GetFileName(this->_filePath);
	for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
		const auto &material = scene->mMaterials[i];
		auto mat = std::make_shared<Material>(this->_shader);

		material->Get(AI_MATKEY_SHININESS, mat->shiness);
		material->Get(AI_MATKEY_COLOR_DIFFUSE, mat->Diffuse);
		material->Get(AI_MATKEY_COLOR_AMBIENT, mat->Ambient);
		material->Get(AI_MATKEY_COLOR_SPECULAR, mat->Specular);

		if (glm::length(mat->Diffuse) == 0)
			mat->Diffuse = glm::vec3(0.8, 0.8, 0.8);
		if (glm::length(mat->Ambient) == 0)
			mat->Ambient = glm::vec3(0.3, 0.3, 0.3);
		if (glm::length(mat->Specular) == 0)
			mat->Specular = glm::vec3(0.3, 0.3, 0.3);

		if (mat->shiness == 0)
			mat->shiness = 10;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				auto filePath = CleanFilePath(path.data, fileDirectory);
				auto name = GetFileName(path.data);

				auto tex = Scene::CreateTexture(filename + "/" + name);
				tex->initFromFile(filePath);
				mat->AddTexture(tex);
			}
		}
		this->_materials.push_back(mat);
	}

	for (int index = 0; index < scene->mNumMeshes; index++) {
		const auto mesh = scene->mMeshes[index];

		if (!mesh->HasPositions() || !mesh->HasFaces())
			continue;

		auto obj = Scene::CreateObject<TriangleObject>();

		if (mesh->HasPositions()) obj->vertices = std::vector<glm::vec3>(mesh->mNumVertices);
		if (mesh->HasNormals()) obj->normals = std::vector<glm::vec3>(mesh->mNumVertices);
		if (mesh->HasTextureCoords(0)) obj->uvs = std::vector<glm::vec2>(mesh->mNumVertices);

		for (int i = 0; i < mesh->mNumVertices; i++) {
			if (mesh->HasPositions()) {
				const auto &vertex = mesh->mVertices[i];
				obj->vertices[i] = glm::vec3(vertex.x, vertex.y, vertex.z);
			}
			if (mesh->HasNormals()) {
				const auto &normal = mesh->mNormals[i];
				obj->normals[i] = glm::vec3(normal.x, normal.y, normal.z);
			}
			if (mesh->HasTextureCoords(0)) {
				const auto &uv = mesh->mTextureCoords[0][i];
				obj->uvs[i] = glm::vec2(uv.x, uv.y);
			}
		}

		if (mesh->HasFaces()) {
			obj->triangles = std::vector<TriangleObject::TriangleIndex>(mesh->mNumFaces);
			for (int i = 0; i < mesh->mNumFaces; i++) {
				const auto &face = mesh->mFaces[i];

				obj->triangles[i] = TriangleObject::TriangleIndex(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
			}
		}

		if (mesh->mMaterialIndex >= 0)
			obj->material = this->_materials[mesh->mMaterialIndex];

		obj->SetParent(Scene::FindObjectByID(this->getObjectID()));
		obj->setup();
		this->_childObjects.push_back(obj);
	}
}