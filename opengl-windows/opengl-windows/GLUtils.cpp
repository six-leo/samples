#include "pch.h"
#include "GLUtils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include<atlimage.h>
GLUtils::GLUtils()
{
	
}


GLUtils::~GLUtils()
{
}

int GLUtils::LoadShader(int type, const char * source)
{
	int shader = glCreateShader(type);
	if (shader != 0) {
		const char* strings[] = { source };
		glShaderSource(shader, 1, strings, 0);
		glCompileShader(shader);
		int compiled[1];
		glGetShaderiv(shader, GL_COMPILE_STATUS, compiled);
		if (compiled[0] == 0) {
			//loge
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			char* buf = (char*)malloc(infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, buf);
			printf("ERROR Could not compile shader :%d\n", type);
			printf("gl_ERROR%s", buf);
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

int GLUtils::CreateProgram(const char * vertex_source, const char * fragment_source, int * vertex_shader, int * fragment_shader)
{
	const char *version = (const char*)glGetString(GL_VERSION);
	printf("opengl version is:%s\n", version);
	int vertexShader = LoadShader(GL_VERTEX_SHADER, vertex_source);
	if (vertexShader == 0) {
		return 0;
	}
	int pixelShader = LoadShader(GL_FRAGMENT_SHADER, fragment_source);
	if (pixelShader == 0) {
		return 0;
	}
	printf("loader shader end\n");
	*vertex_shader = vertexShader;
	*fragment_shader = pixelShader;
	int program = glCreateProgram();
	if (program != 0) {
		glAttachShader(program, vertexShader);
		CheckGlError("glAttachShader vertexShader");
		glAttachShader(program, pixelShader);
		CheckGlError("glAttachShader pixelShader");
		glLinkProgram(program);
		int linkStatus[1];
		glGetProgramiv(program, GL_LINK_STATUS, linkStatus);
		if (linkStatus[0] != GL_TRUE) {

			printf("gl_ERROR Could not link program: \n");
			glDeleteProgram(program);
			program = 0;
			vertex_shader = 0;
			fragment_shader = 0;

		}
	}
	printf("gl create program is end: \n");
	return program;
}

int GLUtils::CheckGlError(const char * op)
{
	int error = 0;
	int haserr = 0;
	while ((error = glGetError()) != GL_NO_ERROR) {
		//logE
		printf("ES20_ERROR:%s: glError:%d\n", op, error);
		haserr = -1;
	}
	return haserr;
}


GLuint GLUtils::ATLLoadTexture(const char *fileName)
{
	BITMAP bm;
	GLuint idTexture = 0;
	CImage img;				//需要头文件atlimage.h
	int num = MultiByteToWideChar(0, 0, fileName, -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(0, 0, fileName, -1, wide, num);
	HRESULT hr = img.Load(wide);
	if (!SUCCEEDED(hr))	//文件加载失败
	{
		MessageBox(NULL, L"文件加载失败", L"ERROR", 0);
		return NULL;
	}
	HBITMAP hbmp = img;
	if (!GetObject(hbmp, sizeof(bm), &bm))
		return 0;

	glGenTextures(1, &idTexture);
	if (idTexture)
	{
		glBindTexture(GL_TEXTURE_2D, idTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glPixelStoref(GL_PACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bm.bmWidth, bm.bmHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, bm.bmBits);	//这里不是GL_RGB
	}
	return idTexture;

}