#pragma once
class TriangleRender
{
public:
	TriangleRender();
	~TriangleRender();

	int Init();
	int Destory();
	int Draw(int textureId);

private:
	int program;
	int vertexShader;
	int fragmentShader;
	int mvboId;
	int posAttributePosition;
	int utexture;
};

