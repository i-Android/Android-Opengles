#include "LWGLRender.h"
#include "LWGLDefine.h"
#include <time.h>


int LWGLRender::renderWithProgram(GLuint *textures, InputFormat inputFormat,
								 RotationMode inputRotation, const RectF *pInputRect,
								 LWGLShaderProgram *glProgram, int outWidth, int outHeight, int targetWidth,
								 int targetHeight)
{
    long t = clock();
//    LOGD("GLFilter::renderWithProgram textures:%d inputRotation:%d pInputRect:%x\n", textures[0], inputRotation, pInputRect);
//    checkGlError("before GLFilter::renderWithProgram");
    if (textures == NULL
    		|| textures[0] == INVALID_TEXTURE
    		|| !mInitialized
    		|| outWidth <= 0
    		|| outHeight <= 0
    		|| targetWidth <= 0
    		|| targetHeight <= 0)
    {
        return -1;
    }

    GLfloat ratioX = ((GLfloat) outWidth) / targetWidth;
    GLfloat ratioY = ((GLfloat) outHeight) / targetHeight;
//    GLContext::getInstance()->setActiveShaderProgram(glProgram);

    glViewport(0, 0, targetWidth, targetHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // set attrbutes
    glVertexAttribPointer(glProgram->getPositionAttribute(), 2, GL_FLOAT, GL_FALSE, 0, imageVertices(RotationMode_Rotate0_FlipV, ratioX, ratioY));
    glEnableVertexAttribArray(glProgram->getPositionAttribute());
    glVertexAttribPointer(glProgram->getTextureCoordinateAttribute(), 2, GL_FLOAT, GL_FALSE, 0, textureCoordinates(inputRotation, pInputRect));
    glEnableVertexAttribArray(glProgram->getTextureCoordinateAttribute());
    glProgram->onAttachAttributes();  // setAttributes(glProgram->getAttributes());

    // set uniforms
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glProgram->getInputTextureUniform(), GL_TEXTURE0 - GL_TEXTURE0);
    if (inputFormat == InputFormat_RGB || inputFormat == InputFormat_RGBA) {
        glProgram->onAttachUniforms();
    } else if (inputFormat == InputFormat_YUV420P) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glUniform1i(glProgram->getInputUTextureUniform(), GL_TEXTURE1 - GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textures[2]);
        glUniform1i(glProgram->getInputVTextureUniform(), GL_TEXTURE2 - GL_TEXTURE0);

        glProgram->onAttachUniforms();
    }
    // process

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // restore
    glDisableVertexAttribArray(glProgram->getPositionAttribute());
    glDisableVertexAttribArray(glProgram->getTextureCoordinateAttribute());
//    GLContext::getInstance()->setActiveShaderProgram(NULL);

    LOGI("GLFilter::renderWithProgram out delta time:%f\n", (clock() - t) / 1000.0);
//    checkGlError("after GLFilter::renderWithProgram");


    return 0;
}


const GLfloat *LWGLRender::imageVertices(RotationMode rotation, GLfloat ratioX, GLfloat ratioY)
{
    static GLfloat *imageVertices = new GLfloat[8];
    switch (rotation)
    {
        case RotationMode_Rotate0:

//            -1,  -1,
//             1,  -1,
//            -1,   1,
//             1,   1,
            imageVertices[0] = imageVertices[4] = -ratioX;
            imageVertices[2] = imageVertices[6] = ratioX;
            imageVertices[1] = imageVertices[3] = -ratioY;
            imageVertices[5] = imageVertices[7] = ratioY;
            break;

        case RotationMode_Rotate0_FlipV:

//            -1,   1
//             1,   1
//            -1,  -1
//             1,  -1
            imageVertices[0] = imageVertices[4] = -ratioX;
            imageVertices[2] = imageVertices[6] = ratioX;
            imageVertices[1] = imageVertices[3] = ratioY;
            imageVertices[5] = imageVertices[7] = -ratioY;
            break;
    }

    return imageVertices;
}


const GLfloat *LWGLRender::textureCoordinates(RotationMode rotation, const RectF *pInputRect)
{
    // pInputRect represents input rectangle after rotation
    RectF rect = { 0, 0, 1, 1 };

    if (pInputRect)
    {
        rect = *pInputRect;
    }

    static GLfloat *textureCoordinates = new GLfloat[8];
    switch (rotation)
    {
        case RotationMode_Rotate0: // verified
//            0.0f, 1.0f,
//            1.0f, 1.0f,
//            0.0f, 0.0f,
//            1.0f, 0.0f,
            textureCoordinates[0] = textureCoordinates[4] = rect.left;
            textureCoordinates[2] = textureCoordinates[6] = rect.right;
            textureCoordinates[1] = textureCoordinates[3] = 1 - rect.top;
            textureCoordinates[5] = textureCoordinates[7] = 1 - rect.bottom;
            break;

        case RotationMode_Rotate90:
//            1.0f, 1.0f,
//            1.0f, 0.0f,
//            0.0f, 1.0f,
//            0.0f, 0.0f,
            textureCoordinates[0] = textureCoordinates[2] = 1 - rect.top;
            textureCoordinates[4] = textureCoordinates[6] = 1 - rect.bottom;
            textureCoordinates[1] = textureCoordinates[5] = 1 - rect.left;
            textureCoordinates[3] = textureCoordinates[7] = 1 - rect.right;
            break;

        case RotationMode_Rotate270: // verified
//            1.0f, 1.0f,
//            1.0f, 0.0f,
//            0.0f, 1.0f,
//            0.0f, 0.0f,
            textureCoordinates[0] = textureCoordinates[2] = rect.top;
            textureCoordinates[4] = textureCoordinates[6] = rect.bottom;
            textureCoordinates[1] = textureCoordinates[5] = rect.left;
            textureCoordinates[3] = textureCoordinates[7] = rect.right;
            break;

        case RotationMode_Rotate0_FlipV: // verified
//            0.0f, 0.0f,
//            1.0f, 0.0f,
//            0.0f, 1.0f,
//            1.0f, 1.0f,
            // swap top & bottom
            textureCoordinates[0] = textureCoordinates[4] = rect.left;
            textureCoordinates[2] = textureCoordinates[6] = rect.right;
            textureCoordinates[1] = textureCoordinates[3] = rect.top;
            textureCoordinates[5] = textureCoordinates[7] = rect.bottom;
            break;

        case RotationMode_Rotate180FlipV: // verified
//            1.0f, 1.0f,
//            0.0f, 1.0f,
//            1.0f, 0.0f,
//            0.0f, 0.0f,
            textureCoordinates[0] = textureCoordinates[4] = 1 - rect.left;
            textureCoordinates[2] = textureCoordinates[6] = 1 - rect.right;
            textureCoordinates[1] = textureCoordinates[3] = 1 - rect.top;
            textureCoordinates[5] = textureCoordinates[7] = 1 - rect.bottom;
            break;

        case RotationMode_Rotate270FlipV: // verified
//            1.0f, 0.0f,
//            1.0f, 1.0f,
//            0.0f, 0.0f,
//            0.0f, 1.0f,
            textureCoordinates[0] = textureCoordinates[2] = 1 - rect.top;
            textureCoordinates[4] = textureCoordinates[6] = 1 - rect.bottom;
            textureCoordinates[1] = textureCoordinates[5] = rect.left;
            textureCoordinates[3] = textureCoordinates[7] = rect.right;
            break;

        case RotationMode_Rotate180: // verified
//            1.0f, 0.0f,
//            0.0f, 0.0f,
//            1.0f, 1.0f,
//            0.0f, 1.0f,
            textureCoordinates[0] = textureCoordinates[4] = 1 - rect.left;
            textureCoordinates[2] = textureCoordinates[6] = 1 - rect.right;
            textureCoordinates[1] = textureCoordinates[3] = rect.top;
            textureCoordinates[5] = textureCoordinates[7] = rect.bottom;
            break;

        case RotationMode_Rotate90_FlipV: // verified
//            0.0f, 1.0f,
//            0.0f, 0.0f,
//            1.0f, 1.0f,
//            1.0f, 0.0f,
            textureCoordinates[0] = textureCoordinates[2] = rect.top;
            textureCoordinates[4] = textureCoordinates[6] = rect.bottom;
            textureCoordinates[1] = textureCoordinates[5] = 1 - rect.left;
            textureCoordinates[3] = textureCoordinates[7] = 1 - rect.right;
            break;
    }
    return textureCoordinates;
}
