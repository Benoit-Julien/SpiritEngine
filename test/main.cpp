#include <MyGlWindow.hpp>
#include <Scene.hpp>

#include <Objects/Primitives/CheckeredFloor.hpp>
#include <Objects/Primitives/Cube.hpp>
#include <Objects/Primitives/Sphere.hpp>
#include <Objects/Primitives/Plane.hpp>
#include <Objects/Model3D.hpp>

#include <Objects/Light.hpp>

static int width = 1280;
static int height = 720;

#ifndef SHADERS_DIRECTORY
	#define SHADERS_DIR std::string("")
#else
	#define SHADERS_DIR std::string(SHADERS_DIRECTORY)
#endif

#ifndef MATERIALS_DIRECTORY
	#define MATERIALS_DIR std::string("")
#else
	#define MATERIALS_DIR std::string(MATERIALS_DIRECTORY)
#endif

#ifndef MODELS_DIRECTORY
	#define MODELS_DIR std::string("")
#else
	#define MODELS_DIR std::string(MODELS_DIRECTORY)
#endif

/*
#include "Crane.hpp"
#include <imgui/imgui.h>

static void DrawHelp() {
	if (ImGui::Begin("Camera help")) {
		ImGui::SetWindowSize(ImVec2(400, 300));

		ImGui::Text("Use mouse left click to rotate the camera.");
		ImGui::Text("Use mouse right click to move the camera look point.");
		ImGui::Text("Use mouse wheel to zoom.");

		ImGui::End();
	}
}

int main() {
	auto window = std::make_shared<MyGlWindow>(width, height);
	Crane crane(window);

	{
		Scene::CreateObject<CheckeredFloor>(50, 16);

		auto cube = Scene::CreateObject<Cube>();
		cube->Translate(glm::vec3(3, 1, 5));

		auto cube2 = Scene::CreateObject<Cube>();
		cube2->Translate(glm::vec3(-4, 1, 4));

		auto cube3 = Scene::CreateObject<Cube>();
		cube3->Translate(glm::vec3(-6, 1, 8));
	}

	window->RegisterFrameFunction(&DrawHelp);
	window->Run();
	return 0;
}*/
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
int main() {
	auto window = std::make_shared<MyGlWindow>(width, height);

	Scene::LoadMaterialFile(MATERIALS_DIR+"materials.json");
	{
		auto plane = Scene::CreateObject<Plane>();
		plane->material = Scene::FindMaterial("plane");
		plane->Scale(glm::vec3(5, 1, 5));

		//auto cube = Scene::CreateObject<Cube>();
		//cube->material = Scene::FindMaterial("mat1");

		//auto sphere = Scene::CreateObject<Sphere>();
		//sphere->material = Scene::FindMaterial("mat1");
		//sphere->Translate(glm::vec3(0, 1, 0));

		//auto sphere2 = Scene::CreateObject<Sphere>(*sphere);
		//sphere2->Translate(glm::vec3(0, 2, 0));

		auto model = Scene::CreateObject<Model3D>(MODELS_DIR+"teapot.obj");
		model->material = Scene::FindMaterial("mat1");
	}

	auto light = Scene::CreateLight<Light>(1.0f);
	light->Translate(glm::vec3(2, 2, 0));
	light->Diffuse = glm::vec3(1, 0, 0);
	light->Ambient = glm::vec3(1, 0, 0);
	light->Specular = glm::vec3(1, 0, 0);

	auto light2 = Scene::CreateLight<Light>(1.0f);
	light2->Translate(glm::vec3(-2, 2, 0));
	light2->Diffuse = glm::vec3(0, 0, 1);
	light2->Ambient = glm::vec3(0, 0, 1);
	light2->Specular = glm::vec3(0, 0, 1);

	auto func = [light, light2](const DrawInformation &) {
		glm::vec3 pos = light->getPosition();
		light->Translate(-pos);
		light->Rotate(0.1f, glm::vec3(0, 1, 0));
		light->Translate(pos);

		pos = light2->getPosition();
		light2->Translate(-pos);
		light2->Rotate(0.1f, glm::vec3(0, 1, 0));
		light2->Translate(pos);
	};

	window->RegisterFrameFunction(func);
	window->Run();
	return 0;
}

//Spirit Engine
//Crystal Game Engine