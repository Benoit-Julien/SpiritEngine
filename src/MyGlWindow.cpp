#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>
#include <imgui/impl/imgui_impl_glfw.h>
#include <imgui/impl/imgui_impl_opengl3.h>
#include <iostream>
#include <thread>
#include <stb/stb_image.h>

#include "MyGlWindow.hpp"
#include "Scene.hpp"
#include "IDGenerator.hpp"
#include "Viewer.h"
#include "Global.hpp"
#include "FBO/FboManager.hpp"
#include "FBO/TextureManager.hpp"
#include "PostProcessing.hpp"
#include "DeferredShading.hpp"

static float DEFAULT_VIEW_POINT[3] = {5, 5, 5};
static float DEFAULT_VIEW_CENTER[3] = {0, 0, 0};
static float DEFAULT_UP_VECTOR[3] = {0, 1, 0};

static void OnError(int errorCode, const char *msg) {
	throw std::runtime_error(msg);
}

MyGlWindow::MyGlWindow(const int &w, const int &h) : width(w), height(h) {
	this->initialize();

	glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
	glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
	glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

	float aspect = (w / (float) h);
	this->_viewer = std::make_shared<Viewer>(viewPoint, viewCenter, upVector, 45.0f, aspect);

	this->_lbutton_down = false;
	this->_rbutton_down = false;
	this->_cx = 0;
	this->_cy = 0;
	this->_lastMouseX = 0;
	this->_lastMouseY = 0;

	this->_postProcessingName = "";
	this->_drawDepth = false;

	this->near = 0.1f;
	this->far = 500.0f;
	PostProcessing::SetNear(this->near);
	PostProcessing::SetFar(this->far);
}

MyGlWindow::~MyGlWindow() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(this->_window);
	glfwTerminate();
}

unsigned int MyGlWindow::RegisterFrameFunction(const std::function<void(DrawInformation &)> &func) {
	auto id = IDGenerator::getSingleton().generateUniqueId();
	this->_frameFunction[id] = func;
	return id;
}

void MyGlWindow::UnRegisterFrameFunction(const unsigned int &ID) {
	auto elem = this->_frameFunction.find(ID);
	if (elem == this->_frameFunction.end())
		throw std::logic_error("Cannot remove function of id " + std::to_string(ID) + " because it's not registered.");

	this->_frameFunction.erase(elem);
	IDGenerator::getSingleton().removeUniqueId(ID);
}

unsigned int MyGlWindow::RegisterPhysicalUpdateFunction(const std::function<void()> &func) {
	auto id = IDGenerator::getSingleton().generateUniqueId();
	this->_physicalUpdateFunction[id] = func;
	return id;
}

void MyGlWindow::UnRegisterPhysicalUpdateFunction(const unsigned int &ID) {
	auto elem = this->_physicalUpdateFunction.find(ID);
	if (elem == this->_physicalUpdateFunction.end())
		throw std::logic_error("Cannot remove function of id " + std::to_string(ID) + " because it's not registered.");

	this->_physicalUpdateFunction.erase(elem);
	IDGenerator::getSingleton().removeUniqueId(ID);
}

void MyGlWindow::Run() {
	this->_windowOpen = true;

	std::thread physicThread(&MyGlWindow::physicalLoop, this);

	this->drawingLoop();
	this->_windowOpen = false;
	physicThread.join();
}

void MyGlWindow::ChangeWindowsLogo(const std::string &path) {
	GLFWimage icon;
	int channels;

	icon.pixels = stbi_load(path.c_str(), &icon.width, &icon.height, &channels, 0);
	if (!icon.pixels)
		throw std::logic_error("Fail to load logo image : " + path);

	glfwSetWindowIcon(this->_window, 1, &icon);
}

void MyGlWindow::drawingLoop() {
	while (!glfwWindowShouldClose(this->_window)) {
		Scene::BeforeDrawing();

		int display_w, display_h;
		glfwGetFramebufferSize(this->_window, &display_w, &display_h);
		if (display_w != this->width || display_h != this->height) {
			glViewport(0, 0, display_w, display_h); // first two data : offset, next two data : width, height
			this->windowResize(display_w, display_h);
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Scene::DrawObjectsList();
		this->draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(this->_window);

		/* Poll for and process events */
		glfwPollEvents();

		this->mouseDragging(this->width, this->height);
	}
}

void MyGlWindow::physicalLoop() {
	long long waitTime = 100;

	while (this->_windowOpen) {
		Scene::PhysicalUpdate();
		for (auto &physicFunc : this->_physicalUpdateFunction)
			physicFunc.second();
		std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
	}
}

void MyGlWindow::draw() {

	glm::vec3 eye = this->_viewer->getViewPoint();
	glm::vec3 look = this->_viewer->getViewCenter();
	glm::vec3 up = this->_viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);

	glm::mat4 projection = glm::perspective(this->_viewer->getFieldOfView(),
																					this->_viewer->getAspectRatio(),
																					this->near,
																					this->far);
	DrawInformation info = {view, projection, eye};

	for (auto &func : this->_frameFunction)
		func.second(info);

	DeferredShading::Draw(info);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, this->width, this->height);

	if (this->_drawDepth)
		PostProcessing::DrawDepth(DeferredShading::GetTexture("depth"));
	else
		PostProcessing::Draw(DeferredShading::GetTexture("deferredRender"), this->_postProcessingName);
		//PostProcessing::Draw(DeferredShading::GetTexture("gPosition"), this->_postProcessingName);
		//PostProcessing::Draw(DeferredShading::GetTexture("gNormal"), this->_postProcessingName);
		//PostProcessing::Draw(DeferredShading::GetTexture("gAmbientAlbedo"), this->_postProcessingName);
		//PostProcessing::Draw(DeferredShading::GetTexture("gDiffuseAlbedo"), this->_postProcessingName);
		//PostProcessing::Draw(DeferredShading::GetTexture("gSpecular"), this->_postProcessingName);

	//ImGui::ShowDemoWindow();
}

void MyGlWindow::windowResize(int w, int h) {
	this->width = w;
	this->height = h;

	float aspect = (w / (float) h);
	this->_viewer->setAspectRatio(aspect);
	DeferredShading::WindowResize(this->width, this->height);
}

void MyGlWindow::initialize() {
	if (!glfwInit())
		throw std::runtime_error("Fail to initialize OpenGL");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwSetErrorCallback(OnError);

	/* Create a windowed mode window and its OpenGL context */
	this->_window = glfwCreateWindow(width, height, "MyOpenGLWindow", NULL, NULL);
	if (!this->_window) {
		glfwTerminate();
		throw std::runtime_error("Fail to create window");
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();

	const char *glsl_version = "#version 410";

	ImGui_ImplGlfw_InitForOpenGL(this->_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
//ImGui::StyleColorsClassic();
//ImGui::StyleColorsLight();

	glfwMakeContextCurrent(this->_window);
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize OpenGL context.");

	glfwSwapInterval(1); //enable vsync

	std::cout << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glfwSetWindowUserPointer(this->_window, this);

	glfwSetKeyCallback(this->_window, genericCallback(key_callback));
	glfwSetCursorPosCallback(this->_window, genericCallback(cursor_pos_callback));
	glfwSetScrollCallback(this->_window, genericCallback(scroll_callback));
	glfwSetMouseButtonCallback(this->_window, genericCallback(mouse_button_callback));

	DeferredShading::WindowResize(this->width, this->height);

	GLFWimage icon;
	int channels;

	icon.pixels = (unsigned char *) DefaultLogo;
	icon.width = DefaultLogoWidth;
	icon.height = DefaultLogoHeight;
	glfwSetWindowIcon(this->_window, 1, &icon);
}

void MyGlWindow::key_callback(int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(this->_window, GLFW_TRUE);
}

void MyGlWindow::cursor_pos_callback(double xpos, double ypos) {
	_cx = xpos;
	_cy = ypos;
}

void MyGlWindow::scroll_callback(double xoffset, double yoffset) {
	this->_viewer->zoom(yoffset * 0.01);
}

void MyGlWindow::mouse_button_callback(int button, int action, int mods) {
	ImGuiIO &io = ImGui::GetIO();
	if (!io.WantCaptureMouse) {
		if (action == GLFW_PRESS) {
			double xpos, ypos;
			glfwGetCursorPos(this->_window, &xpos, &ypos);
			this->_lastMouseX = xpos;
			this->_lastMouseY = ypos;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (GLFW_PRESS == action)
				this->_lbutton_down = true;
			else if (GLFW_RELEASE == action)
				this->_lbutton_down = false;
		}
		else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (GLFW_PRESS == action)
				this->_rbutton_down = true;
			else if (GLFW_RELEASE == action)
				this->_rbutton_down = false;
		}
	}
}

void MyGlWindow::mouseDragging(double width, double height) {
	if (this->_lbutton_down) {
		float fractionChangeX = static_cast<float>(this->_cx - this->_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(this->_lastMouseY - this->_cy) / static_cast<float>(height);
		this->_viewer->rotate(fractionChangeX, fractionChangeY);
	}
	else if (this->_rbutton_down) {
		float fractionChangeX = static_cast<float>(this->_cx - this->_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(this->_lastMouseY - this->_cy) / static_cast<float>(height);
		this->_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
	}
	this->_lastMouseX = this->_cx;
	this->_lastMouseY = this->_cy;
}