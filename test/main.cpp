#include <imgui/imgui.h>

#include <MyGlWindow.hpp>
#include <Scene.hpp>

#include <Objects/Primitives/CheckeredFloor.hpp>
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
//#include <iostream>
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
		} else
			counter++;

		ImGui::Value("Frame Rate", rate);
		ImGui::End();
	}
}

int main() {
	auto window = std::make_shared<MyGlWindow>(width, height);

	Scene::LoadMaterialFile(MATERIALS_DIR + "materials.json");
	//Scene::LoadMaterialFile(MATERIALS_DIR + "simple.json");
	{
		auto plane = Scene::CreateObject<Plane>();
		plane->material = Scene::FindMaterial("plane");
		plane->Scale(glm::vec3(1000, 1, 1000));
		plane->SetCustomUniform("fogColor", glm::vec3(0.5, 0.5, 0.5));
		plane->SetCustomUniform("minDist", 0.1f);
		plane->SetCustomUniform("maxDist", 10.0f);

		auto dragon = Scene::CreateObject<Mesh>(MODELS_DIR + "dragon.obj");
		dragon->material = Scene::FindMaterial("cartoon");
		dragon->Translate(glm::vec3(0, 1, 0));
		dragon->Scale(glm::vec3(2, 2, 2));
		dragon->EnableCulling();
		dragon->SetCullFaceOption(GL_BACK);
		dragon->SetCustomUniform("fogColor", glm::vec3(0.5, 0.5, 0.5));
		dragon->SetCustomUniform("minDist", 0.1f);
		dragon->SetCustomUniform("maxDist", 10.0f);
/*
		auto teapot = Scene::CreateObject<Mesh>(MODELS_DIR + "teapot.3ds");
		teapot->material = Scene::FindMaterial("cartoon");
		teapot->Translate(glm::vec3(0, 1, 0));
		teapot->Rotate(-90, glm::vec3(1, 0, 0));
		teapot->Scale(glm::vec3(0.05, 0.05, 0.05));
		teapot->EnableCulling();
		teapot->SetCullFaceOption(GL_BACK);
		teapot->SetEnable(false);*/

		/*auto silhouette = Scene::CreateObject<Mesh>(*dragon);
		silhouette->material = Scene::FindMaterial("silhouette");
		silhouette->SetCullFaceOption(GL_FRONT);
		silhouette->SetCustomUniform("offset", 0.01f);*/
		//Scene::Destroy(silhouette, 2);

		//window->RegisterFrameFunction(&DrawFrameRate);

		/*auto cow = Scene::CreateObject<Mesh>(MODELS_DIR + "cow.obj");
		cow->material = Scene::FindMaterial("cartoon");
		cow->Translate(glm::vec3(1, 1, 0));
		cow->Rotate(90, glm::vec3(0, 1, 0));
		cow->Scale(glm::vec3(0.25, 0.25, 0.25));
		cow->EnableCulling();
		cow->SetCullFaceOption(GL_BACK);

		auto silhouette2 = Scene::CreateObject<Mesh>(*cow);
		silhouette2->material = Scene::FindMaterial("silhouette");
		silhouette2->SetCullFaceOption(GL_FRONT);
		silhouette2->SetCustomUniform("offset", 0.05f);*/

		//auto light = Scene::CreateLight<Light>();
		//light->Translate(glm::vec3(0, 4, 0));
		auto spot = Scene::CreateLight<SpotLight>(glm::vec3(0, 0, 0), 2);
		auto spot2 = Scene::CreateLight<SpotLight>(*spot);
		spot->Translate(glm::vec3(4, 4, 0));
		spot2->Translate(glm::vec3(-4, 4, 0));
		//}
		/*
		auto model2 = Scene::CreateObject<Mesh>(MODELS_DIR + "buddha.obj");
		model2->material = Scene::FindMaterial("gold");
		model2->Translate(glm::vec3(0, 1, 3));
		model2->Scale(glm::vec3(3, 3, 3));
		model2->Rotate(-90, glm::vec3(0, 1, 0));

		auto model3 = Scene::CreateObject<Mesh>(MODELS_DIR + "bunny.obj");
		model3->material = Scene::FindMaterial("ruby");
		model3->Translate(glm::vec3(0, 0, -3));
		model3->Rotate(90, glm::vec3(0, 1, 0));*/
	}
/*
	glm::vec3 colors[] = {
					glm::vec3(0.0f, 0.8f, 0.8f),
					glm::vec3(0.0f, 0.0f, 0.8f),
					glm::vec3(0.8f, 0.0f, 0.0f),
					glm::vec3(0.0f, 0.8f, 0.0f),
					glm::vec3(0.8f, 0.8f, 0.8f)
	};

	float height = 1;
	float intensity = 1;
	float radius = 1;
	std::vector<std::shared_ptr<Light>> lights;

	for (auto i = 0; i < 5; i++) {
		auto light = Scene::CreateLight<Light>(intensity);
		auto angle = glm::radians(72.0f * i);
		light->Diffuse = light->Ambient = light->Specular = colors[i];
		light->Translate(glm::vec3(radius * glm::cos(angle), height, radius * glm::sin(angle)));
		lights.push_back(light);
	}

	auto func = [&](const DrawInformation &) {
		if (ImGui::Begin("Light")) {
			ImGui::SliderFloat("Lights height", &height, 0, 10);
			ImGui::SliderFloat("Light Intensity", &intensity, 0, 20);
			ImGui::SliderFloat("Light Radius", &radius, 0, 5);
		}
		ImGui::End();

		int i = 0;
		for (auto &light : lights) {
			light->SetIntensity(intensity);

			auto angle = glm::radians(72.0f * i++);
			light->Translate(-light->getPosition());
			light->Translate(glm::vec3(radius * glm::cos(angle), height, radius * glm::sin(angle)));
		}
	};
 	window->RegisterFrameFunction(func);
*/

	/*glm::vec3 color = light->Diffuse;
	glm::vec3 pos = light->getPosition();
	float intensity = light->GetIntensity();
	if (ImGui::Begin("Light")) {
		ImGui::SliderFloat3("Light Position", glm::value_ptr(pos), -10, 10);
		ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
		ImGui::SliderFloat("Light Intensity", &intensity, 0, 20);
		ImGui::End();
	}
	light->Translate(-light->getPosition());
	light->Translate(pos);

	light->Diffuse = color;
	light->Ambient = color;
	light->Specular = color;
	light->SetIntensity(intensity);
};*/

	window->Run();
	return 0;
}