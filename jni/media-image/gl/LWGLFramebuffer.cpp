#include "LWGLFramebuffer.h"

LWGLFrameBuffer::LWGLFrameBuffer() :
		mFramebuffer(INVALID_FRAMEBUFFER),
		mTexture(INVALID_TEXTURE),
		mOldFBO(INVALID_FRAMEBUFFER),
		mWidth(480),
		mHeight(480),
		mRotation(RotationMode_Rotate0),
		mInitialized(false){

}

LWGLFrameBuffer::~LWGLFrameBuffer() {

	glDeleteFramebuffers(1, &mFramebuffer);
}

int LWGLFrameBuffer::initial()
{
	if (mInitialized) {
		destroy();
	}

	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFBO);

	glGenFramebuffers(1, &mFramebuffer);
	glGenTextures(1, &mTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTexture, 0);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	LOGI("FBO Initialed");
	if (status == GL_FRAMEBUFFER_COMPLETE) {
		mInitialized = true;
	} else {
		LOGE("OpenGL error:%x", status);
		if (mTexture != INVALID_TEXTURE) {
			LOGI("RenderTexture::init delete texture %d\n", mTexture);
			glDeleteTextures(1, &mTexture);
			mTexture = INVALID_TEXTURE;
		}
		if (mFramebuffer != INVALID_FRAMEBUFFER) {
			glDeleteFramebuffers(1, &mFramebuffer);
			mFramebuffer = INVALID_FRAMEBUFFER;
		}
		mOldFBO = INVALID_FRAMEBUFFER;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, mOldFBO);
	return mInitialized;
}

int LWGLFrameBuffer::destroy()
{
	if (mTexture != INVALID_TEXTURE)
	{
		LOGI("RenderTexture::uninit delete texture %d\n", mTexture);
		glDeleteTextures(1, &mTexture);
		mTexture = INVALID_TEXTURE;
	}
	if (mFramebuffer != INVALID_FRAMEBUFFER)
	{
		glDeleteFramebuffers(1, &mFramebuffer);
		mFramebuffer = INVALID_FRAMEBUFFER;
	}

	mOldFBO = INVALID_FRAMEBUFFER;

	mInitialized = false;

	return 0;
}


int LWGLFrameBuffer::binding()
{
	if (!mInitialized)
	{
		this->initial();
	}

	if (mInitialized)
	{
		LOGI("FBO Binded:%d", mFramebuffer);
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mOldFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mFramebuffer);
	}


	return 0;
}

int LWGLFrameBuffer::dettach()
{
	if (mInitialized)
	{
		LOGI("FBO dettach:%d", mOldFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, mOldFBO);
	}


	return 0;
}
