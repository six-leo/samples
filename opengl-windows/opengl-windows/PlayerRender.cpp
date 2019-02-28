#include "pch.h"
#include "PlayerRender.h"
#include "GLUtils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include<string>
#include"GLUtils.h"
std::string vertexShaderStringPlane = "#version 430\n"
"in vec4 pos;"
"in vec2 aTextureCoord;"
"varying vec2 vTextureCoord;"
"void main() {"
"gl_Position = pos;"
"vTextureCoord = aTextureCoord;"
"}";

std::string fragmentShaderStringPlane = "#version 430\n"
"uniform  sampler2D uTexture;"
"varying vec2 vTextureCoord;"
"out vec4 color;"
"void main() {"
"color = texture2D(uTexture,vTextureCoord);"
"}";


short plane_index[] = { 0, 1, 2, 0, 2, 3 };
float bufferArray[] =
{
	-1.0F, 1.0F, //1

	0.0F, 1.0F,//2

	0.0F, 0.0F, //3

	-1.0F, -1.0F, //1

	0.0F, 0.0F, //2

	0.0F, 1.0F,//3

	1.0F, -1.0F,//1

	1.0F, 0.0F, //2

	1.0F, 1.0F, //3

	1.0F,1.0F,//1

	1.0F, 1.0F,//2

	1.0F,0.0F //3
};
PlayerRender::PlayerRender()
{
	

}


PlayerRender::~PlayerRender()
{
}

int PlayerRender::Init()
{
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return 0;
	}
	if (this->program > 0) {
		this->Destory();
	}


	BuildProgram();
	CreateDatas();

	return 0;
}

int PlayerRender::Destory()
{
	glUseProgram(0);
	if (this->program != 0) {
		glDeleteProgram(this->program);
	}
	if (this->vertexShader != 0) {
		glDeleteShader(this->vertexShader);
	}
	if (this->fragmentShader != 0) {
		glDeleteShader(this->fragmentShader);
	}
	this->program = 0;
	this->vertexShader = 0;
	this->fragmentShader = 0;
	GLuint bufferids[2];
	bufferids[0] = this->mArrayBufferId;
	bufferids[1] = this->mElementBufferId;
	this->mArrayBufferId = 0;
	this->mElementBufferId = 0;
	glDeleteBuffers(1, bufferids);
	printf("destory_panorama end\n");
	return 1;
}

int PlayerRender::Draw(int  textureId)
{
	if (this->program <= 0) {
		return 0;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearColor(0, 1, 0, 1);

	glUseProgram(this->program);
	GLUtils::CheckGlError("glUseProgram");
	glBindBuffer(GL_ARRAY_BUFFER, this->mArrayBufferId);
	GLUtils::CheckGlError("glBindBuffer1");
	glVertexAttribPointer(this->aPosition, 2, GL_FLOAT, 0, 24, (const void*)0);
	GLUtils::CheckGlError("glVertexAttribPointer1");
	glEnableVertexAttribArray(this->aPosition);
	GLUtils::CheckGlError("glEnableVertexAttribArray1");
	glVertexAttribPointer(this->aTextureCoord, 2, GL_FLOAT, 0, 24, (const void*)16);
	GLUtils::CheckGlError("glVertexAttribPointer2");
	glEnableVertexAttribArray(this->aTextureCoord);
	GLUtils::CheckGlError("glEnableVertexAttribArray2");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	GLUtils::CheckGlError("glBindTexture");
	glUniform1i(this->uTextureSampler, 0);
	GLUtils::CheckGlError("glUniform1i");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mElementBufferId);
	GLUtils::CheckGlError("glBindBuffer");
	glDrawElements(GL_TRIANGLES, sizeof(plane_index) / sizeof(short),GL_UNSIGNED_SHORT, (const void*)0);
	GLUtils::CheckGlError("glDrawElements");
	glDisableVertexAttribArray(this->aPosition);
	glDisableVertexAttribArray(this->aTextureCoord);
	glUseProgram(0);
	glFlush();
	return 1;
}





int PlayerRender::BuildProgram()
{
	this->program = GLUtils::CreateProgram(vertexShaderStringPlane.c_str(),fragmentShaderStringPlane.c_str(), &this->vertexShader, &this->fragmentShader);

	if (this->program > 0) 
	{
		this->aPosition = glGetAttribLocation(this->program,"pos");
		this->aTextureCoord = glGetAttribLocation(this->program,"aTextureCoord");
		this->uTextureSampler = glGetUniformLocation(this->program, "uTextureSampler");
	}

	return this->program;
}

int PlayerRender::CreateDatas()
{
	GLuint bufferIds[2];
	glGenBuffers(2, bufferIds);
	this->mArrayBufferId = bufferIds[0];
	this->mElementBufferId = bufferIds[1];
	glBindBuffer(34962, this->mArrayBufferId);
	GLUtils::CheckGlError("glBindTexture");
	glBufferData(34962, sizeof(bufferArray), bufferArray, 35044);
	GLUtils::CheckGlError("glBufferData");
	glBindBuffer(34962, this->mElementBufferId);
	GLUtils::CheckGlError("glBindBuffer");
	glBufferData(34962, sizeof(plane_index), plane_index, 35044);
	GLUtils::CheckGlError("glBufferData");
	glBindBuffer(34962, 0);
	glBindBuffer(34963, 0);
	return 1;
}
