#pragma once

class PlayerRender
{
public:
	PlayerRender();
	~PlayerRender();


	

	int Init();
	int Destory();
	int Draw(int textureId);

private:
	int type;//(0,letf-right,1,up-down,2 single)
	int isReverse;//��ת
	int textureType;//texture2d,(android e texture_oes);

	// These will always be > 0 after a build, any errors will abort()
	int program;
	int vertexShader;
	int fragmentShader;

	//attributes that aren't found will have a -1 value
	int aPosition;
	int aTextureCoord;
	int uLayout;

	// Uniforms that aren't found will have a -1 value
	int uTextureSampler;

	int mArrayBufferId;
	int mElementBufferId;


	int BuildProgram();
	int CreateDatas();

};

