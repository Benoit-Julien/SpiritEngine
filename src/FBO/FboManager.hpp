#ifndef SPIRITENGINE_FBO_MANAGER_HPP
#define SPIRITENGINE_FBO_MANAGER_HPP

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

class FboManager {
 public:
	explicit FboManager();
 	~FboManager();

	//check fbo status
	bool checkFboStatus();

	//create a renderbuffer object
	void genRenderDepthBuffer(const unsigned &w, const unsigned &h);

	//bind a renderbuffer object
	void bindRenderDepthBuffer();

	void bindToFbo(const GLenum &type, const GLenum &texture, const GLuint &textureId);
	void bind3DTextureToFbo(const GLenum &type, const GLuint &textureId);
	void setDrawBuffers();
	inline const GLuint &getFboId() { return this->_fboId; }

 private:
	GLuint _fboId;
	GLuint _renderDepthBufferId;
	bool useRenderDepthBuffer;
	unsigned attachmentCount;
	GLenum mrt[6];
};

#endif