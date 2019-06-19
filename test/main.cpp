#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <random>
#include <string>

#include <MyGlWindow.hpp>
#include <Scene.hpp>

#include <Objects/Mesh.hpp>
#include <Objects/Lights/Light.hpp>
#include <Objects/Lights/SpotLight.hpp>

#include <PostProcessing.hpp>
#include <DeferredShading.hpp>

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

static void DrawHelp(DrawInformation &) {
	if (ImGui::Begin("Camera help")) {
		ImGui::SetWindowSize(ImVec2(400, 100));

		ImGui::Text("Use mouse left click to rotate the camera.");
		ImGui::Text("Use mouse right click to move the camera look point.");
		ImGui::Text("Use mouse wheel to zoom.");
	}
	ImGui::End();
}

static auto chrono = std::chrono::system_clock::now() + std::chrono::milliseconds(250);
static int counter = 0;

static void DrawFrameRate(DrawInformation &) {
	static int rate = 0;

	if (std::chrono::system_clock::now() > chrono) {
		rate = counter * 4;
		chrono = std::chrono::system_clock::now() + std::chrono::milliseconds(250);
		counter = 0;
	}
	else
		counter++;
	if (ImGui::Begin("Frame Rate")) {
		ImGui::SetWindowSize(ImVec2(150, 50));
		ImGui::Value("Frame Rate", rate);
	}
	ImGui::End();
}

static void DirectionalLightInfo() {
	if (!Scene::GetDirectionLight() && ImGui::Button("Create"))
		Scene::CreateDirectionalLight();
	else if (Scene::GetDirectionLight() && ImGui::Button("Destroy"))
		Scene::DestroyDirectionalLight();

	if (Scene::GetDirectionLight()) {
		static glm::vec3 euler;
		static float minIntensity = 0;

		if (ImGui::DragFloat2("Rotation", &euler.x))
			Scene::GetDirectionLight()->SetEulerAngles(glm::radians(euler));

		ImGui::ColorEdit3("Ambient", &Scene::GetDirectionLight()->Ambient.x);
		ImGui::ColorEdit3("Diffuse", &Scene::GetDirectionLight()->Diffuse.x);
		ImGui::ColorEdit3("Specular", &Scene::GetDirectionLight()->Specular.x);
		ImGui::DragScalar("Intensity", ImGuiDataType_Float, &Scene::GetDirectionLight()->Intensity, 0.01f, &minIntensity, nullptr, "%.3f");
	}
}

static void SpotLightInfo(std::shared_ptr<Mesh> mountain) {
	static std::vector<std::pair<std::shared_ptr<SpotLight>, glm::vec3>> spots;
	static float minIntensity = 0;

	int index = 0;
	std::vector<std::pair<int, std::shared_ptr<SpotLight>>> toDelete;
	for (auto light : spots)
	{
		if (ImGui::TreeNode((void*)(intptr_t)light.first->getObjectID(), "Spot %d", index))
		{
			ImGui::DragFloat3("Position", &light.first->GetPosition().x);

			if (ImGui::DragFloat3("Rotation", &light.second.x))
				light.first->SetEulerAngles(glm::radians(light.second));

			ImGui::ColorEdit3("Ambient", &light.first->Ambient.x);
			ImGui::ColorEdit3("Diffuse", &light.first->Diffuse.x);
			ImGui::ColorEdit3("Specular", &light.first->Specular.x);
			ImGui::DragScalar("Intensity", ImGuiDataType_Float, &light.first->Intensity, 0.01f, &minIntensity, nullptr, "%.3f");

			ImGui::DragFloat("Cutoff", &light.first->Cutoff, 0.1f);
			ImGui::DragFloat("InnerCutoff", &light.first->InnerCutoff, 0.1f);
			ImGui::DragFloat("Exponent", &light.first->Exponent, 0.1f);

			if (ImGui::Button("Remove"))
				toDelete.emplace_back(index, light.first);

			ImGui::TreePop();
		}
		index++;
	}

	if (ImGui::Button("Create")) {
		auto l = Scene::CreateLight<SpotLight>();
		spots.emplace_back(l, glm::vec3(0, 0, 0));
	}

	for (auto light : toDelete) {
		spots.erase(spots.begin()+light.first);
		Scene::RemoveLight(light.second->getObjectID());
	}
}

static void PointLightInfo(std::shared_ptr<Mesh> mountain) {
	static int lightNb = 0;
	static int minLightNb = 0;
	static std::random_device rd;

	if (ImGui::DragScalar("Light Number", ImGuiDataType_S32, &lightNb, .1f, &minLightNb, nullptr, "%d")) {
		auto lights = Scene::GetLights();

		int diff = lightNb - lights.size();
		if (diff < 0) {
			std::vector<unsigned int> toDelete;

			auto it = lights.cbegin();
			for (auto i = 0; i < lights.size() + diff; i++, ++it);
			for (; it != lights.cend(); ++it)
				toDelete.push_back(it->first);
			for (auto &l : toDelete)
				Scene::RemoveLight(l);
		}
		else {
			for (auto i = 0; i < diff; i++) {
				auto newLight = Scene::CreateLight<Light>();
				auto mesh = std::dynamic_pointer_cast<TriangleObject>(mountain->Children.front());

				auto vertIndex = rd() % mesh->vertices.size();
				auto m = mountain->GetWorldModelMatrix();
				auto n = glm::mat3(glm::transpose(glm::inverse(m)));

				auto normal = n * mesh->normals[vertIndex];
				auto pos = m * glm::vec4(mesh->vertices[vertIndex], 1);

				newLight->SetPosition(glm::vec3(pos) + normal);

				auto color = glm::vec3(rd() % 255, rd() % 255, rd() % 255);
				newLight->Diffuse = color;
				newLight->Ambient = color * 0.01f;
				newLight->Intensity = 0.01f;
				newLight->UpdateLightVolume();
			}
		}
	}
}

static void LightingInfo(std::shared_ptr<Mesh> mountain, DrawInformation &) {
	if (ImGui::Begin("Lighting")) {
		if (ImGui::CollapsingHeader("Directional")) DirectionalLightInfo();

		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Point")) PointLightInfo(mountain);

		ImGui::Spacing();
		if (ImGui::CollapsingHeader("Spot")) SpotLightInfo(mountain);
	}
	ImGui::End();
}

static void DrawingInfo(std::shared_ptr<MyGlWindow> window, DrawInformation &) {
	static bool drawDepth = false;
	static const char *postProcessingEffect[] = {"default", "blur", "sharpening", "greyscale", "sephia", "sobel"};
	static const char *gBuffer[] = {"default", "gPosition", "gNormal", "gAmbientAlbedo", "gDiffuseAlbedo", "gSpecular"};
	static int postProcessingIndex = 0;
	static int gBufferIndex;

	if (ImGui::Begin("Post Processing")) {
		ImGui::SetWindowSize(ImVec2(290, 120));
		ImGui::Checkbox("Depth Map", &drawDepth);
		ImGui::Combo("Effect", &postProcessingIndex, postProcessingEffect, IM_ARRAYSIZE(postProcessingEffect));
		ImGui::Combo("Buffer", &gBufferIndex, gBuffer, IM_ARRAYSIZE(gBuffer));
	}
	ImGui::End();

	window->DrawDepth(drawDepth);

	if (postProcessingIndex == 0)
		window->ResetPostProcessing();
	else
		window->SetPostProcessing(postProcessingEffect[postProcessingIndex]);

	if (gBufferIndex == 0)
		window->SetDrawBuffer("deferredRender");
	else
		window->SetDrawBuffer(gBuffer[gBufferIndex]);
}

int main() {
	auto window = std::make_shared<MyGlWindow>(width, height);

	Scene::LoadShaderFile(SHADERS_DIR + "shaders.json");
	Scene::LoadTextureFile(TEXTURES_DIR + "textures.json");
	Scene::LoadMaterialFile(MATERIALS_DIR + "materials.json");
	Scene::LoadMaterialFile(MATERIALS_DIR + "simple.json");
	{
		auto mountain = Scene::CreateMesh("mountain");
		mountain->LoadMesh(MODELS_DIR + "mountain/mount.blend1.obj");
		mountain->Scale(glm::vec3(40, 20, 40));

		/*auto sponza = Scene::CreateMesh("sponza");
		sponza->LoadMesh(MODELS_DIR + "Sponza/sponza.obj");
		sponza->Scale(glm::vec3(0.1, 0.1, 0.1));*/

		window->RegisterFrameFunction(std::bind(&LightingInfo, mountain, std::placeholders::_1));
		window->RegisterFrameFunction(std::bind(&DrawingInfo, window, std::placeholders::_1));

		window->RegisterFrameFunction(DrawFrameRate);
		window->RegisterFrameFunction(DrawHelp);
	}

	window->Run();
	return 0;
}