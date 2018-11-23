#include <assimp/scene.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h>     // Post processing flags
#include <exception>

#include "Model3D.hpp"

Model3D::Model3D(const std::string &filePath)
				: Drawable(ObjectType::Model3D),
					_filePath(filePath),
					_drawMode(DrawMode::Shape) {
	this->setup();
}

Model3D::~Model3D() {}

Model3D::Model3D(const Model3D &model)
				: Drawable(model),
					_filePath(model._filePath),
					_drawMode(model._drawMode),
					_childObjects(model._childObjects.size()) {
	for (auto i = 0; i < model._childObjects.size(); i++)
		this->_childObjects[i] = std::make_shared<TriangleObject>(*model._childObjects[i]);
}

Model3D &Model3D::operator=(const Model3D &model) {
	this->_filePath = model._filePath;
	this->_drawMode = model._drawMode;
	this->_childObjects = std::vector<std::shared_ptr<TriangleObject>>(model._childObjects.size());
	for (auto i = 0; i < model._childObjects.size(); i++)
		this->_childObjects[i] = std::make_shared<TriangleObject>(*model._childObjects[i]);
	return *this;
}

void Model3D::Draw() {
	for (auto &obj : this->_childObjects)
		obj->Draw();
}

void Model3D::setDrawMode(const DrawMode &mode) {
	_drawMode = mode;
	for (auto &obj : this->_childObjects)
		obj->setDrawMode(mode);
}

void Model3D::setup() {
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll
	// probably to request more postprocessing than we do in this example.
	const aiScene *scene = importer.ReadFile(this->_filePath, aiProcessPreset_TargetRealtime_Fast);
	if (!scene)
		throw std::logic_error("Fail to load model file: " + this->_filePath);

	for (int index = 0; index < scene->mNumMeshes; index++) {
		const auto mesh = scene->mMeshes[index];

		if (!mesh->HasPositions() || !mesh->HasFaces())
			continue;

		std::shared_ptr<TriangleObject> obj = std::make_shared<TriangleObject>();

		obj->vertices = std::vector<glm::vec3>(mesh->mNumVertices);
		obj->normals = std::vector<glm::vec3>(mesh->mNumVertices);
		for (int i = 0; i < mesh->mNumVertices; i++) {
			const auto &vertex = mesh->mVertices[i];
			const auto &normal = mesh->mNormals[i];

			obj->vertices[i] = glm::vec3(vertex.x, vertex.y, vertex.z);
			obj->normals[i] = glm::vec3(normal.x, normal.y, normal.z);
		}

		obj->triangles = std::vector<TriangleObject::TriangleIndex>(mesh->mNumFaces);
		for (int i = 0; i < mesh->mNumFaces; i++) {
			const auto &face = mesh->mFaces[i];

			obj->triangles[i] = TriangleObject::TriangleIndex(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
		}
		obj->setup();
		this->_childObjects.push_back(obj);
	}
}