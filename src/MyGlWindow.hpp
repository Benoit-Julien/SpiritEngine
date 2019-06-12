#ifndef COMPUTERGRAPHICS1_MYGLWINDOW_HPP
#define COMPUTERGRAPHICS1_MYGLWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <unordered_map>
#include <functional>

#include "Objects/Object.hpp"
#include "Viewer.h"
#include "Scene.hpp"

#define genericCallback(functionName)\
        [](GLFWwindow* window, auto... args) {\
            auto pointer = static_cast<MyGlWindow*>(glfwGetWindowUserPointer(window));\
            if (pointer) pointer->functionName(args...);\
        }

class MyGlWindow {
 private:
	GLFWwindow *_window;
	std::shared_ptr<Viewer> _viewer;

	std::unordered_map<unsigned int, std::function<void(DrawInformation &)>> _frameFunction;
	std::unordered_map<unsigned int, std::function<void()>> _physicalUpdateFunction;

	bool _windowOpen;

	std::string _postProcessingName;
	std::string _bufferName;
	bool _drawDepth;

 public:
	int width;
	int height;
	float near;
	float far;

 private:
	bool _lbutton_down;
	bool _rbutton_down;
	double _lastMouseX;
	double _lastMouseY;
	double _cx, _cy;

 public:
	MyGlWindow(const int &w, const int &h);
	~MyGlWindow();

	void Run();

	unsigned int RegisterFrameFunction(const std::function<void(DrawInformation &)> &func);
	void UnRegisterFrameFunction(const unsigned int &ID);

	unsigned int RegisterPhysicalUpdateFunction(const std::function<void()> &func);
	void UnRegisterPhysicalUpdateFunction(const unsigned int &ID);

	inline void SetPostProcessing(const std::string &name) { this->_postProcessingName = name; }
	inline void ResetPostProcessing() { this->_postProcessingName = ""; }

	inline void SetDrawBuffer(const std::string &name) { this->_bufferName = name; }

	inline void DrawDepth(const bool &depth) { this->_drawDepth = depth; }

	void ChangeWindowsLogo(const std::string &path);

 private:
	void physicalLoop();
	void drawingLoop();

	void initialize();
	void draw();

	void key_callback(int key, int scancode, int action, int mods);
	void windowResize(int w, int h);

	void cursor_pos_callback(double xpos, double ypos);
	void scroll_callback(double xoffset, double yoffset);
	void mouse_button_callback(int button, int action, int mods);

	void mouseDragging(double width, double height);
};


#endif /* !COMPUTERGRAPHICS1_MYGLWINDOW_HPP */
