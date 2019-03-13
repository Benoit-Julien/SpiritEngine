#ifndef COMPUTERGRAPHICS1_MYGLWINDOW_HPP
#define COMPUTERGRAPHICS1_MYGLWINDOW_HPP

#include <GL/glew.h>
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

	std::unordered_map<unsigned int, std::function<void (DrawInformation &)>> _frameFunction;

	bool _windowOpen;

 public:
	int width;
	int height;

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
	unsigned int RegisterFrameFunction(const std::function<void (DrawInformation &)> &func);
	void UnRegisterFrameFunction(const unsigned int &ID);

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
