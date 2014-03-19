#ifndef __GL_MANAGER_H__
#define __GL_MANAGER_H__

#include "LWGLShaderProgram.h"
#include "LWGLRender.h"
#include "LWGLFramebuffer.h"


class LWGLManager {


public:
	static LWGLManager* getInstance();
private:
	static LWGLManager* mPLWGLManager;

public:

	int initial();
	int render();
	int drawTexture(GLuint textureId);

	int setInputSource(int texture, int width, int height, int rotation);
	int drawTest();
	int drawTest2();
private:

	LWGLShaderProgram 	mLWGLProgram;
	LWGLFrameBuffer 	mLWGLFrameBuffer;


	RotationMode mRotation;
	GLuint mInputTexture;
	int mInputWidth;
	int mInputHeight;


};

#endif // __GL_PROGRAM_H__
