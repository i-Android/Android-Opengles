#ifndef __GL_RENDER_H__
#define __GL_RENDER_H__

#include "LWGLDefine.h"
#include "LWGLShaderProgram.h"
#include <vector>
#include <map>
#include <string>

/**
 * filter primitive. by desheng.jd
 */
class LWGLRender {

public:

    static const GLfloat *imageVertices(RotationMode rotation, GLfloat ratioX, GLfloat ratioY);
    static const GLfloat *textureCoordinates(RotationMode rotationMode, const RectF *pInputRect);
public:

//	void renderToTarget(GLuint *textures, InputFormat inputFormat, int inputWidth, int inputHeight, RotationMode rotation, const RectF *pInputRect, int targetWidth, int targetHeight, Rect *pOutRect);

	int renderWithProgram(	GLuint *textures,
							InputFormat inputFormat,
							RotationMode inputRotation,
							const RectF *pInputRect,
							LWGLShaderProgram *glProgram,
							int outWidth, int outHeight,
							int targetWidth, int targetHeight);

private:
	bool mInitialized;
};

#endif // __GL_PROGRAM_H__
