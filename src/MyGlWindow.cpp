#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/impl/imgui_impl_glfw.h>
#include <imgui/impl/imgui_impl_opengl3.h>
#include <iostream>

#include "MyGlWindow.hpp"
#include "Scene.hpp"
#include "IDGenerator.hpp"
#include "Viewer.h"

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

void MyGlWindow::Run() {
	while (!glfwWindowShouldClose(this->_window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0); /// background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

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

		this->draw();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Swap front and back buffers */
		glfwSwapBuffers(this->_window);
		/* Poll for and process events */
		glfwPollEvents();

		this->mouseDragging(display_w, display_h);
	}
}

void MyGlWindow::draw() {
	glm::vec3 eye = this->_viewer->getViewPoint();
	glm::vec3 look = this->_viewer->getViewCenter();
	glm::vec3 up = this->_viewer->getUpVector();
	glm::mat4 view = glm::lookAt(eye, look, up);

	glm::mat4 projection = glm::perspective(this->_viewer->getFieldOfView(),
																					this->_viewer->getAspectRatio(),
																					0.1f,
																					500.0f);
	DrawInformation info = { view, projection };
	for (auto &func : this->_frameFunction)
		func.second(info);

	Scene::Draw(info);
}

void MyGlWindow::windowResize(int w, int h) {
	this->width = w;
	this->height = h;

	float aspect = (w / (float) h);
	this->_viewer->setAspectRatio(aspect);
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
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	const char* glsl_version = "#version 410";

	ImGui_ImplGlfw_InitForOpenGL(this->_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImGui::StyleColorsDark();
//ImGui::StyleColorsClassic();
//ImGui::StyleColorsLight();

	glfwMakeContextCurrent(this->_window);
	/* Make the window's context current */
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		//Problem: glewInit failed, something is seriously wrong.
		throw std::runtime_error("glewInit failed, aborting.");
	}
	glfwSwapInterval(1); //enable vsync

	std::cout << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glfwSetWindowUserPointer(this->_window, this);

	glfwSetKeyCallback(this->_window, genericCallback(key_callback));
	glfwSetCursorPosCallback(this->_window, genericCallback(cursor_pos_callback));
	glfwSetScrollCallback(this->_window, genericCallback(scroll_callback));
	glfwSetMouseButtonCallback(this->_window, genericCallback(mouse_button_callback));
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
	this->_viewer->zoom(yoffset * 0.01f);
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
	if (_lbutton_down) {
		float fractionChangeX = static_cast<float>(this->_cx - this->_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(this->_lastMouseY - this->_cy) / static_cast<float>(height);
		this->_viewer->rotate(fractionChangeX, fractionChangeY);
	}
	else if (_rbutton_down) {
		float fractionChangeX = static_cast<float>(this->_cx - this->_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(this->_lastMouseY - this->_cy) / static_cast<float>(height);
		this->_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
	}
	this->_lastMouseX = this->_cx;
	this->_lastMouseY = this->_cy;
}