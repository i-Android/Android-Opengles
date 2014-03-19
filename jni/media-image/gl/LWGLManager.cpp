#include "LWGLManager.h"
#include "LWGLDefine.h"


LWGLManager* LWGLManager::mPLWGLManager = nullptr;

LWGLManager* LWGLManager::getInstance() {
	if (nullptr==mPLWGLManager) {
		mPLWGLManager = new LWGLManager();
	}

	return mPLWGLManager;
};


int LWGLManager::initial() {

	mLWGLProgram.initProgram();
	mLWGLFrameBuffer.initial();

	return 0;
}

int LWGLManager::setInputSource(int texture, int width, int height, int rotation) {
	mInputTexture = texture;
	if (mInputTexture != INVALID_TEXTURE && width > 0 && height > 0)
	{
		mInputWidth = width;
		mInputHeight = height;

		mRotation = rotation;
	}
	return 0;
}

int LWGLManager::drawTest() {

	GLfloat vVertices[] = {	0.0f, 0.5f, 0.0f,
							-0.5f, -0.5f, 0.0f,
							0.5f, -0.5f, 0.0f};

	// Set the viewport
	glViewport( 0, 0, 480, 480);
	// Clear the color buffer
	glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// set uniforms
	glActiveTexture(GL_TEXTURE0);
	LOGE("drawTest texture:%d", mLWGLFrameBuffer.mTexture);
	glBindTexture(GL_TEXTURE_2D, mLWGLFrameBuffer.mTexture);
	glUniform1i(mLWGLProgram.getShaderProgram(), 0);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glDisableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);


	return 0;
}


int LWGLManager::drawTest2() {

	GLfloat vVertices[] = {	0.2f, 0.5f, 0.0f,
							-0.5f, -0.5f, 0.0f,
							0.5f, -0.5f, 0.0f};

	// Set the viewport
	glViewport( 0, 0, 480, 480);
	// Clear the color buffer
	glClear( GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// set uniforms
	glActiveTexture(GL_TEXTURE0);
	LOGE("drawTest2 texture:%d", mLWGLFrameBuffer.mTexture);
	glBindTexture(GL_TEXTURE_2D, mLWGLFrameBuffer.mTexture);
	glUniform1i(mLWGLProgram.getShaderProgram(), 0);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glDisableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);


	return 0;
}

int LWGLManager::drawTexture(GLuint textureId) {
	// set attrbutes
	LOGE("drawTexture postion:%d", mLWGLProgram.getPositionAttribute());
	glVertexAttribPointer(mLWGLProgram.getPositionAttribute(), 2, GL_FLOAT, GL_FALSE, 0, LWGLRender::imageVertices(RotationMode_Rotate0_FlipV, 1.0, 1.0));
	glEnableVertexAttribArray(mLWGLProgram.getPositionAttribute());
	LOGE("drawTexture coord:%d", mLWGLProgram.getTextureCoordinateAttribute());
	glVertexAttribPointer(mLWGLProgram.getTextureCoordinateAttribute(), 2, GL_FLOAT, GL_FALSE, 0, LWGLRender::textureCoordinates(RotationMode_Rotate0, NULL));
	glEnableVertexAttribArray(mLWGLProgram.getTextureCoordinateAttribute());

	// set uniforms
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,  textureId);
	LOGE("drawTexture texture:%d", textureId);
	glUniform1i(mLWGLProgram.getInputTextureUniform(), 0);

	// process
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// restore
	glDisableVertexAttribArray(mLWGLProgram.getPositionAttribute());
	glDisableVertexAttribArray(mLWGLProgram.getTextureCoordinateAttribute());
	glBindTexture(GL_TEXTURE_2D, 0);

	return 0;
}

int LWGLManager::render() {

	LOGE("opengl render");


	mLWGLProgram.doneProgram();
	mLWGLFrameBuffer.binding();

	drawTexture(this->mInputTexture);
//	drawTest();
//
//
	mLWGLFrameBuffer.dettach();

//	mLWGLProgram.doneProgram();
//
//	drawTest2();

	drawTexture(mLWGLFrameBuffer.getTexture());

//	drawTexture(this->mInputTexture);

	return 0;
}
