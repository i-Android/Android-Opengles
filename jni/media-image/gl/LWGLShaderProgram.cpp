#include "LWGLShaderProgram.h"
#include "LWGLDefine.h"
#include "LWGLHelper.h"

#include <string>


const char vShaderStr[] =
		"attribute vec4 vPosition;									\n"
		"attribute vec4 vInputTextureCoordinate;					\n"
		"varying vec2 vTextureCoordinate;							\n"
		"void main()												\n"
		"{															\n"
		"	gl_Position = vPosition;								\n"
		"	vTextureCoordinate = vInputTextureCoordinate.xy;		\n"
		"}";

const char fShaderStr[] =
		"varying highp vec2 vTextureCoordinate;						\n"
		"uniform sampler2D vInputImageTexture;						\n"
		"void main()												\n"
		"{															\n"
		"	gl_FragColor = texture2D(vInputImageTexture, vTextureCoordinate); \n"
		"}";

//const GLbyte vShaderStr[] =
//		"attribute vec4 vPosition; 				\n"
//		"void main() 							\n"
//		"{ 										\n"
//		" 	gl_Position = vPosition; 			\n"
//		"} 										\n";
//
//const GLbyte fShaderStr[] =
//		"precision mediump float; 				\n"
//		"void main() 							\n"
//		"{ 										\n"
//		" 	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
//		"} 										\n";

LWGLShaderProgram::LWGLShaderProgram () :
    	    mEGLProgram(INVALID_PROGRAM),
    	    mPositionAttribute(INVALID_ATTRIBUTE_INDEX),
    	    mTextureCoordinateAttribute(INVALID_ATTRIBUTE_INDEX),
    	    mInputTextureUniform(INVALID_UNIFORM),
    	    mInitialized(false),
    	    mAttributeIndex(0)
{

}

int LWGLShaderProgram::initProgram() {

	mEGLProgram = glCreateProgram();

	GLuint vertexShader 	= loadShader(vShaderStr, GL_VERTEX_SHADER);
	GLuint fragmentShader 	= loadShader(fShaderStr, GL_FRAGMENT_SHADER);
	LOGE("initProgram vertexShader:%d", vertexShader);
	glAttachShader(mEGLProgram, vertexShader);
	glAttachShader(mEGLProgram, fragmentShader);

	checkGlError("glAttachShader");
	mPositionAttribute = mAttributeIndex++;
	glBindAttribLocation(mEGLProgram, mPositionAttribute, "vPosition");
	checkGlError("glBindAttribLocation");
	mTextureCoordinateAttribute = mAttributeIndex++;
	glBindAttribLocation(mEGLProgram, mTextureCoordinateAttribute, "vInputTextureCoordinate");

	LOGE("initProgram mPositionAttribute:%d", mPositionAttribute);

	linkProgram();
	mInputTextureUniform = glGetUniformLocation(mEGLProgram, "vInputImageTexture");

	LOGE("linkProgram");
	if (INVALID_SHADER != vertexShader)
	{
		glDeleteShader(vertexShader);
		vertexShader = INVALID_SHADER;
	}
	if (INVALID_SHADER != fragmentShader)
	{
		glDeleteShader(fragmentShader);
		fragmentShader = INVALID_SHADER;
	}


//	doneProgram();

//	GLint vertexPositionAttribute	= glGetAttribLocation(mLWGLProgram, "_VertexPosition");
//	glEnableVertexAttribArray( vertexPositionAttribute );
//
//	GLint vertexColorAttribute	= glGetAttribLocation(mLWGLProgram, "_VertexColor");
//	glEnableVertexAttribArray(vertexColorAttribute);

//	glGetUniformLocation( mLWGLProgram, "uPMatrix"); // please check errors
//	glGetUniformLocation( mLWGLProgram, "uMVMatrix");

	return 0;
}


GLuint LWGLShaderProgram::loadShader(const char *shaderString, GLenum type) {

	GLuint shader = glCreateShader(type);
	if (shader) {

		glShaderSource(shader, 1, &shaderString, 0);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled) {

			GLint infoLen;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen > 0) {
				GLchar *buf = (GLchar *) malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, &infoLen, buf);
					LOGE("Shader compile log:\n%s", buf);
					free(buf);
				}
			}
			glDeleteShader(shader);
			shader = INVALID_SHADER;
		}
	}
	return shader;
}


int LWGLShaderProgram::linkProgram() {
	GLint status;

	glLinkProgram(mEGLProgram);

	glGetProgramiv(mEGLProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE) {

		GLint logLength;
		glGetProgramiv(mEGLProgram, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0) {
			GLchar *log = (GLchar *) malloc(logLength);
			glGetProgramInfoLog(mEGLProgram, logLength, &logLength, log);
			LOGE("Could not link program:\n%s\n", log);
			free(log);
		}
		glDeleteProgram(mEGLProgram);
		mEGLProgram = INVALID_PROGRAM;
	}

	return mEGLProgram;
}

int LWGLShaderProgram::doneProgram() {
	glUseProgram(mEGLProgram);
	return 0;
}

int LWGLShaderProgram::exitProgram() {

	glDeleteProgram(mEGLProgram);
	mEGLProgram = INVALID_PROGRAM;
	return 0;
}

int LWGLShaderProgram::onAttachAttributes() {
	//    for (vector<PGLParam>::iterator iter = mAttributes.begin(); iter != mAttributes.end(); iter++)
	//    {
	//        PGLParam var = *iter;
	//        switch (var->getType())
	//        {
	//            case VAR_FLOAT4:
	//                glVertexAttribPointer(var->getLocation(), 2, GL_FLOAT, GL_FALSE, 0, var->getGLValue());
	//                glEnableVertexAttribArray(var->getLocation());
	//                break;
	//        }
	//    }


	return 0;
}

int LWGLShaderProgram::onAttachUniforms() {
	//    int textureOffset = 1;
	//
	//    for (vector<PGLParam>::iterator iter = mUniforms.begin(); iter != mUniforms.end(); iter++)
	//    {
	//        PGLParam var = *iter;
	//        switch (var->getGLType())
	//        {
	//            case VAR_GL_SAMPLER_2D:
	//                glActiveTexture(GL_TEXTURE0 + textureOffset);
	//                glBindTexture(GL_TEXTURE_2D, *((GLuint *) var->getGLValue()));
	//                glUniform1i(var->getLocation(), textureOffset);
	//                textureOffset++;
	//                break;
	//
	//            case VAR_INT:
	//                glUniform1iv(var->getLocation(), 1, (GLint *) var->getGLValue());
	//                break;
	//
	//            case VAR_INT2:
	//                glUniform2iv(var->getLocation(), 1, (GLint *) var->getGLValue());
	//                break;
	//
	//            case VAR_INT3:
	//                glUniform3iv(var->getLocation(), 1, (GLint *) var->getGLValue());
	//                break;
	//
	//            case VAR_INT4:
	//                glUniform4iv(var->getLocation(), 1, (GLint *) var->getGLValue());
	//                break;
	//
	//            case VAR_FLOAT:
	//                glUniform1fv(var->getLocation(), 1, (GLfloat *) var->getGLValue());
	//                break;
	//
	//            case VAR_FLOAT2:
	//                glUniform2fv(var->getLocation(), 1, (GLfloat *) var->getGLValue());
	//                break;
	//
	//            case VAR_FLOAT3:
	//                glUniform3fv(var->getLocation(), 1, (GLfloat *) var->getGLValue());
	//                break;
	//
	//            case VAR_FLOAT4:
	//                glUniform4fv(var->getLocation(), 1, (GLfloat *) var->getGLValue());
	//                break;
	//        }
	//    }


	return 0;
}

int LWGLShaderProgram::onContextChanged() {
	//    mGLProgram = INVALID_PROGRAM;
	//    mPositionAttribute = INVALID_ATTRIBUTE_INDEX;
	//    mTextureCoordinateAttribute = INVALID_ATTRIBUTE_INDEX;
	//    mInputTextureUniform = INVALID_UNIFORM;
	//
	//    for (vector<PGLParam>::iterator iter = mAttributes.begin(); iter != mAttributes.end(); iter++)
	//    {
	//        (*iter)->onContextChanged();
	//    }
	//    mAttributes.clear();
	//    mAttributeIndex = 0;
	//
	//    for (vector<PGLParam>::iterator iter = mUniforms.begin(); iter != mUniforms.end(); iter++)
	//    {
	//        (*iter)->onContextChanged();
	//    }
	//    mUniforms.clear();
	//
	//    mInitialized = false;


	return 0;
}

