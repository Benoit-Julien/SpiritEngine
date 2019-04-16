#ifndef SPIRITENGINE_SKYBOX_HPP
#define SPIRITENGINE_SKYBOX_HPP

#include "../TriangleObject.hpp"

class SkyBox : public TriangleObject {
 public:
	explicit SkyBox();
	virtual ~SkyBox();

	SkyBox(const SkyBox &skyBox);
	SkyBox &operator=(const SkyBox &skyBox);

 private:
	void setupVertices();

	virtual void beforeDraw();
	virtual void afterDraw();
};

#endif /* !SPIRITENGINE_SKYBOX_HPP */
