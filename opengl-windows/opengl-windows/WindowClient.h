#pragma once
#include<d3d11.h>
namespace LeoPlayer
{

	class WindowClient 
	{
	public: 
		WindowClient();
		~WindowClient();
		int Create(int x,int y,int width, int height,bool isNeedD3d =true);
		void ShowWindow(bool isShow);
		HANDLE GetShareHandle();
		HWND GetWindowId();
		ID3D11Texture2D* GetWindowBackBuffer();
		ID3D11Texture2D* GetWindowTexture2D();
		//ID3D11Texture2D* GetWindowTexture2DExtend();
		ID3D11DeviceContext*GetDeviceContext();
		ID3D11Device *GetDevice();
		
		void UpdateToShareTexture();
		void UpdateToWindow();
		void Present();
		static LRESULT  MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	private:
		bool isCreated = false;
		HWND mHwd;
		ID3D11Texture2D *mBackBuffer;
		ID3D11Texture2D *mSharedTexture;
		//ID3D11Texture2D *mSharedTextureExtend;
		DXGI_FORMAT mFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
		ID3D11Device* mDevice;
		ID3D11DeviceContext* mDeviceContext;
		IDXGISwapChain *mSwapChain;
	};

}