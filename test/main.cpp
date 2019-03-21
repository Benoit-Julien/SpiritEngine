#include <imgui/imgui.h>

#include <MyGlWindow.hpp>
#include <Scene.hpp>

#include <Objects/Primitives/Cube.hpp>
#include <Objects/Primitives/Sphere.hpp>
#include <Objects/Primitives/Plane.hpp>
#include <Objects/Mesh.hpp>

#include <Objects/Lights/Light.hpp>
#include <Objects/Lights/SpotLight.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>

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

#ifndef TEXTURES_DIRECTORY
#define TEXTURES_DIR std::string("")
#else
#define TEXTURES_DIR std::string(TEXTURES_DIRECTORY)
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
}*/

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/string_cast.hpp>

static auto chrono = std::chrono::system_clock::now() + std::chrono::milliseconds(250);
static int counter = 0;

static void DrawFrameRate(DrawInformation &info) {
	static int rate = 0;

	if (ImGui::Begin("Frame Rate")) {
		if (std::chrono::system_clock::now() > chrono) {
			rate = counter * 4;
			chrono = std::chrono::system_clock::now() + std::chrono::milliseconds(250);
			counter = 0;
		}
		else
			counter++;

		ImGui::Value("Frame Rate", rate);
		ImGui::End();
	}
}

int main() {
	auto window = std::make_shared<MyGlWindow>(width, height);

	Scene::LoadShaderFile(SHADERS_DIR + "shaders.json");
	Scene::LoadTextureFile(TEXTURES_DIR + "textures.json");
	Scene::LoadMaterialFile(MATERIALS_DIR + "materials.json");
	//Scene::LoadMaterialFile(MATERIALS_DIR + "simple.json");
	{
		/*auto plane = Scene::CreateObject<Plane>();
		plane->material = Scene::FindMaterial("plane");
		plane->Scale(glm::vec3(1000, 1, 1000));
		plane->SetCustomUniform("fogColor", glm::vec3(0.5, 0.5, 0.5));
		plane->SetCustomUniform("minDist", 0.1f);
		plane->SetCustomUniform("maxDist", 10.0f);

		auto cube = Scene::CreateObject<Cube>();
		cube->Translate(glm::vec3(3, 1.5, 0));
		cube->material = Scene::FindMaterial("brick");

		auto world = Scene::CreateObject<Sphere>(1, 48, 32);
		world->Translate(glm::vec3(0, 2, 0));
		world->Rotate(180, glm::vec3(1, 0, 0));
		world->material = Scene::FindMaterial("earth");


		window->RegisterPhysicalUpdateFunction([world]() {
			world->Rotate(0.0001, glm::vec3(0, 1, 0));
		});
*/
		/*auto mountain = Scene::CreateMesh("mountain");
		mountain->SetShader("textured.vert", "textured.frag");
		mountain->LoadMesh(MODELS_DIR + "mountain/mount.blend1.obj");*/

		auto sponza = Scene::CreateMesh("sponza");
		sponza->SetShader("textured.vert", "textured.frag");
		sponza->LoadMesh(MODELS_DIR + "Sponza/sponza.obj");
		sponza->Scale(glm::vec3(0.1, 0.1, 0.1));

		window->RegisterFrameFunction(DrawFrameRate);

		/*auto spot = Scene::CreateLight<SpotLight>(glm::vec3(0, 0, 0), 2);
		auto spot2 = Scene::CreateLight<SpotLight>(*spot);
		spot->Translate(glm::vec3(4, 4, 0));
		spot2->Translate(glm::vec3(-4, 4, 0));*/

		auto light = Scene::CreateLight<Light>();
		light->Translate(glm::vec3(0, 6, 0));
		light->SetIntensity(4);

		/*auto light2 = Scene::CreateLight<Light>();
		light2->Translate(glm::vec3(-4, 4, 0));

		auto light3 = Scene::CreateLight<Light>();
		light3->Translate(glm::vec3(0, 4, 4));

		auto light4 = Scene::CreateLight<Light>();
		light4->Translate(glm::vec3(0, 4, -4));*/

/*
		auto dragon = Scene::CreateObject<Mesh>(MODELS_DIR + "dragon.obj");
		dragon->material = Scene::FindMaterial("cartoon");
		dragon->Translate(glm::vec3(0, 1, 0));
		dragon->Scale(glm::vec3(2, 2, 2));
		dragon->EnableCulling();
		dragon->SetCullFaceOption(GL_BACK);
		dragon->SetCustomUniform("fogColor", glm::vec3(0.5, 0.5, 0.5));
		dragon->SetCustomUniform("minDist", 0.1f);
		dragon->SetCustomUniform("maxDist", 10.0f);

		auto silhouette = Scene::CreateObject<Mesh>(*dragon);
		silhouette->material = Scene::FindMaterial("silhouette");
		silhouette->SetCullFaceOption(GL_FRONT);
		silhouette->SetCustomUniform("offset", 0.01f);*/
	}

	window->Run();
	return 0;
}