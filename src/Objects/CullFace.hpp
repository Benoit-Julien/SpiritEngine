#ifndef SPIRITENGINE_CULLFACE_HPP
#define SPIRITENGINE_CULLFACE_HPP

#include <glad/glad.h>

class CullFace {
	bool _enabledCulling;
	GLenum _cullface;

 public:
	explicit CullFace();
	CullFace(const CullFace &cull);
	CullFace &operator=(const CullFace &cull);
	~CullFace();

	virtual void SetCullFaceOption(const GLenum &cullface);
	inline const GLenum &GetCullFaceOption() const { return this->_cullface; }

	void EnableCulling();
	virtual void DisableCulling();
	inline const bool &Culling() const { return this->_enabledCulling; }
};

#endif /* !SPIRITENGINE_CULLFACE_HPP */
