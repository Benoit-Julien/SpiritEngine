#ifndef COMPUTERGRAPHICS1_CRANE_HPP
#define COMPUTERGRAPHICS1_CRANE_HPP

#include <memory>

#include <Objects/Primitives/WireCube.hpp>
#include <Objects/Primitives/Cube.hpp>
#include <Objects/LineSegment.hpp>

#include <MyGlWindow.hpp>
#include <Scene.hpp>

class Crane {
	std::shared_ptr<MyGlWindow> _window;

	const unsigned int _height;
	const unsigned int _width;

	glm::vec3 _position;

	std::shared_ptr<WireCube> _root;
	std::shared_ptr<WireCube> _boomPivo;
	std::shared_ptr<LineSegment> _cable;
	std::shared_ptr<Cube> _grapObject;

	float _boomRotation;
	float _boomH;
	float _boomV;

	bool _defaultMode;
	bool _boxMode;

 public:
	Crane(std::shared_ptr<MyGlWindow> window, const unsigned int &height = 7, const unsigned int &width = 7);
	~Crane();

 private:
	void createCrane();

	void frame(DrawInformation &);
	void updateLookAt(DrawInformation &info);
};


#endif /* !COMPUTERGRAPHICS1_CRANE_HPP */
