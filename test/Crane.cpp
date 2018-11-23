#include <imgui/imgui.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Crane.hpp"
#include "Scene.hpp"

Crane::Crane(std::shared_ptr<MyGlWindow> window, const unsigned int &height, const unsigned int &width)
				: _window(window),
					_height(height),
					_width(width) {
	this->createCrane();

	this->_boomRotation = 0;
	this->_boomH = 0;
	this->_boomV = 0;

	this->_defaultMode = true;
	this->_boxMode = false;
	this->_window->RegisterFrameFunction(std::bind(&Crane::frame, this, std::placeholders::_1));
	this->_window->RegisterFrameFunction(std::bind(&Crane::updateLookAt, this, std::placeholders::_1));
}

Crane::~Crane() {}

void Crane::frame(DrawInformation &) {
	if (ImGui::Begin("Control Window")) {
		ImGui::SetWindowSize(ImVec2(400, 300));

		ImGui::Text("Crane Control");
		ImGui::SliderFloat("Boom Rotation", &this->_boomRotation, -180, 180);
		ImGui::SliderFloat("Boom Horizontal", &this->_boomH, 0, 1);
		ImGui::SliderFloat("Boom Vertical", &this->_boomV, 0, 1);

		if (ImGui::RadioButton("Default", this->_defaultMode)) {
			this->_defaultMode = true;
			this->_boxMode = false;
			std::cout << "Active default mode" << std::endl;
			this->_grapObject->Enabled = true;
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Box", this->_boxMode)) {
			this->_defaultMode = false;
			this->_boxMode = true;
			std::cout << "Active box mode" << std::endl;
			this->_grapObject->Enabled = false;
		}
		ImGui::End();
	}
	this->_root->Translate(-this->_root->getPosition());
	this->_root->Translate(this->_position);

	this->_boomPivo->Rotate(glm::inverse(this->_boomPivo->getRotation()));
	this->_boomPivo->Rotate(this->_boomRotation, glm::vec3(0, 1, 0));

	float maxH = this->_width * 2 - 2;
	float hValue = 2 + (maxH * this->_boomH);
	this->_cable->Translate(-this->_cable->getPosition());
	this->_cable->Translate(glm::vec3(hValue, -1, 0));

	float maxV = this->_height * 2 - 2;
	float vValue = -(1 + maxV * this->_boomV);
	this->_cable->Scale(glm::vec3(1, 1 / this->_cable->getScale().y, 1));
	this->_cable->Scale(glm::vec3(1, vValue, 1));

	glm::vec3 pos = this->_cable->ToWorldPosition();
	glm::vec3 sc = this->_cable->ToWorldScale();

	this->_grapObject->Rotate(glm::inverse(this->_grapObject->getRotation()));
	this->_grapObject->Translate(-this->_grapObject->getPosition());

	this->_grapObject->Translate(glm::vec3(pos.x, vValue - 1, pos.z));
	this->_grapObject->Rotate(this->_boomPivo->ToWorldRotation());
}

void Crane::updateLookAt(DrawInformation &info) {
	if (!this->_boxMode)
		return;

	glm::vec3 pos = this->_cable->ToWorldPosition();
	float maxV = this->_height * 2 - 2;
	float vValue = -(1 + maxV * this->_boomV);

	glm::vec3 campos = glm::vec3(pos.x, this->_boomPivo->ToWorldPosition().y + vValue - 1, pos.z);
	glm::vec3 dir = this->_boomPivo->ToWorldRotation() * glm::vec4(1, 0, 0, 0);

	info.viewMatrix = glm::lookAt(campos, campos + dir, glm::vec3(0, 1, 0));
}

void Crane::createCrane() {
	this->_root = Scene::CreateObject<WireCube>();
	this->_root->Translate(glm::vec3(0, 1, 0));
	this->_position = this->_root->getPosition();

	for (auto i = 1; i < this->_height; i++) {
		auto obj = Scene::CreateObject<WireCube>();
		obj->SetParent(this->_root);
		obj->Translate(glm::vec3(0, 2 * i, 0));
	}

	this->_boomPivo = Scene::CreateObject<WireCube>();
	this->_boomPivo->SetParent(this->_root);
	this->_boomPivo->Translate(glm::vec3(0, 2 * this->_height, 0));

	auto cube = Scene::CreateObject<WireCube>();
	cube->SetParent(this->_boomPivo);
	cube->Translate(glm::vec3(-2, 0, 0));

	for (auto i = 0; i < this->_width; i++) {
		auto obj = Scene::CreateObject<WireCube>();
		obj->SetParent(this->_boomPivo);
		obj->Translate(glm::vec3(2 * (i + 1), 0, 0));
	}

	this->_cable = Scene::CreateObject<LineSegment>();
	this->_cable->SetParent(this->_boomPivo);
	this->_cable->Translate(glm::vec3(2, -1, 0));
	this->_cable->Scale(glm::vec3(1, 1, 1));

	this->_grapObject = Scene::CreateObject<ColorCube>();
}