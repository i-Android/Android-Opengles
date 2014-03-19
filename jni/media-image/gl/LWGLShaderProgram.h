#ifndef __LWGL_SHADERPROGRAM_H__
#define __LWGL_SHADERPROGRAM_H__

#include "LWGLDefine.h"

class LWGLShaderParam {

protected:
    GLint mLocation;
    GLint mLocationU;
    GLint mLocationV;

    GLuint mCreatedTexture;
};

class LWGLShaderProgram {

public:
	LWGLShaderProgram();

public:
	inline GLint getPositionAttribute() { return mPositionAttribute; }
	inline GLint getTextureCoordinateAttribute() { return mTextureCoordinateAttribute; }
	inline GLint getInputTextureUniform() { return mInputTextureUniform; }
	inline GLint getInputUTextureUniform() { return mInputUTextureUniform; }
	inline GLint getInputVTextureUniform() { return mInputVTextureUniform; }
	inline GLint getShaderProgram() { return mEGLProgram; }

public:

	GLuint loadShader(const char *shaderString, GLenum type);

	int initProgram();
	int linkProgram();
	int doneProgram();
	int exitProgram();


    int onAttachAttributes();
    int onAttachUniforms();
    int onContextChanged();

private:
	GLuint mEGLProgram;

    GLint mPositionAttribute;
    GLint mTextureCoordinateAttribute;
    GLint mInputTextureUniform;  		// y texture for yuv input or rgb texture for rgb input
    GLint mInputUTextureUniform;
    GLint mInputVTextureUniform;
    GLint mAttributeIndex;

    bool mInitialized;
};

#endif // __LWGL_SHADERPROGRAM_H__
