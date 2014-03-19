#ifndef __LWGL_FRAMEBUFFER_H__
#define __LWGL_FRAMEBUFFER_H__

#include "LWGLDefine.h"


/**
 * FBO Object Manager, by desheng.jd
 */
class LWGLFrameBuffer {
public:
	LWGLFrameBuffer();
	~LWGLFrameBuffer();
public:
	auto initial() 	-> int;
	auto destroy() 	-> int;
	auto binding() 	-> int;
	auto dettach()	-> int;
	auto checkStatus() -> int;

	auto getTexture() -> GLuint {return mTexture;};

public:
	GLuint 	mFramebuffer;
	GLuint 	mTexture;
	GLint 	mOldFBO;
	int mWidth;
	int mHeight;
	bool mInitialized;
	RotationMode mRotation;
};


class LWGLRenderTextrue {

};

#endif // __LWGL_FRAMEBUFFER_H__
