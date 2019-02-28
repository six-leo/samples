#pragma once
#include<GL/glew.h>
class GLUtils
{
public:
	GLUtils();
	~GLUtils();

	static int LoadShader(int type, const char *source);

	static int CreateProgram(const char*vertexSource, const char*fragmentSource, int *vertex_shader, int*fragment_shader);

	static int CheckGlError(const char* op);

	static GLuint ATLLoadTexture(const char *fileName);
};

