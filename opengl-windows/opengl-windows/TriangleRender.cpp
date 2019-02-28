#include "pch.h"
#include "TriangleRender.h"
#include<string>
#include"GLUtils.h"
std::string vertexShaderString = "#version 430\n"
"in vec3 pos;"
"void main() {"
"gl_Position = vec4(pos, 1);"
"}";

std::string fragmentShaderString = "#version 430\n"
"uniform  sampler2D uTexture;"
"out vec4 color;"
"void main() {"
"color = texture2D(uTexture,vec2(0.5,0.5));"
"}";

TriangleRender::TriangleRender()
{
}


TriangleRender::~TriangleRender()
{
}

int TriangleRender::Init()
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
	this->program = GLUtils::CreateProgram(vertexShaderString.c_str(), fragmentShaderString.c_str(), &this->vertexShader, &this->fragmentShader);
	posAttributePosition = glGetAttribLocation(program, "pos");
	utexture = glGetUniformLocation(this->program, "uTexture");
	GLfloat vertices[] = { // vertex coordinates 
						   -0.7, -0.7, 0,
						   0.7, -0.7, 0,
						   0, 0.7, 0
	};
	GLuint vboId;
	// allocate buffer sapce and pass data to it 
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	mvboId = vboId;
	// unbind the active buffer 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return 0;
}

int TriangleRender::Destory()
{

	
	


	return 0;
}

int TriangleRender::Draw(int textureId)
{
	if (this->program < 0) {
		return 0;
	}

	GLuint vaoId;
	glGenVertexArrays(1, &vaoId); // Generate VAO 

	// Bind it so that rest of vao operations affect this vao 
	glBindVertexArray(vaoId);

	// buffer from which 'pos' will recive its data and the format of that data 
	glBindBuffer(GL_ARRAY_BUFFER, mvboId);
	glVertexAttribPointer(posAttributePosition, 3, GL_FLOAT, false, 0, 0);

	// Enable this attribute array linked to 'pos' 
	glEnableVertexAttribArray(posAttributePosition);
	glUseProgram(program);
	// clear the color buffer before each drawing 
	glClear(GL_COLOR_BUFFER_BIT);

	// draw triangles starting from index 0 and 
	// using 3 indices 
	glDrawArrays(GL_TRIANGLES, 0, 3);

	return 0;
}
