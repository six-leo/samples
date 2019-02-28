#include "pch.h"
#include "WindowClient.h"
#include<stdio.h>
#include<tchar.h>
#include <DXGI.h>
#pragma comment ( lib, "D3D11.lib")

namespace LeoPlayer {
	WindowClient::WindowClient()
	{
	}
	WindowClient::~WindowClient()
	{
	}
	int WindowClient::Create(int x, int y, int width, int height, bool isNeedD3d)
	{
		if (isCreated)
			return -1;
		// create window (after we know the size of the input file size)
		WNDCLASSEX wc = { sizeof(WNDCLASSEX),  CS_HREDRAW | CS_VREDRAW, MsgProc, 0L, 0L,
			GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
			_T("tmp"), NULL
		};
		RegisterClassEx(&wc);
		// figure out the window size we must create to get a *client* area
			// that is of the size requested by m_dimensions.
		RECT adjustedWindowSize;
		DWORD dwWindowStyle;

		dwWindowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		SetRect(&adjustedWindowSize, x, y, width+x, height+y);
		AdjustWindowRect(&adjustedWindowSize, dwWindowStyle, false);

		int windowWidth = adjustedWindowSize.right - adjustedWindowSize.left;
		int windowHeight = adjustedWindowSize.bottom - adjustedWindowSize.top;

		mHwd = CreateWindow(wc.lpszClassName, _T("tmpwin"),
			dwWindowStyle,
			x, y,
			windowWidth,
			windowHeight,
			NULL, NULL, wc.hInstance, NULL);
		if (isNeedD3d == false) {
			return 0;
		}

		if (mSwapChain == NULL) {
			DXGI_SWAP_CHAIN_DESC sc = { 0 };
			sc.BufferCount = 1;
			
			sc.BufferDesc.Width = width;
			sc.BufferDesc.Height = height;
		

			sc.BufferDesc.Format = mFormat;
			sc.BufferDesc.RefreshRate.Numerator = 0;
			sc.BufferDesc.RefreshRate.Denominator = 1;
			sc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//DXGI_USAGE_SHADER_INPUT |DXGI_USAGE_SHARED| | DXGI_USAGE_BACK_BUFFER| DXGI_USAGE_DISCARD_ON_PRESENT;
			sc.OutputWindow = mHwd;
			sc.SampleDesc.Count = 1;
			sc.SampleDesc.Quality = 0;
			sc.Windowed = true;
			sc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			//sc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			sc.Flags = 0;

			HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
				NULL, 0, NULL, 0, D3D11_SDK_VERSION, &sc, &mSwapChain, &mDevice, NULL, &mDeviceContext);

			if (FAILED(hr))
			{
				printf("Unable to create DX11 device and swapchain, hr=0x%x", hr);
				return false;
			}

			HRESULT result;
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = width;
			descDepth.Height = height;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = mFormat;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
			result = mDevice->CreateTexture2D(&descDepth, nullptr, &mSharedTexture);
			if (FAILED(result))
			{
				printf_s("创建sharedtextures失败1\n");
				return false;
			}
		
			
		}
		

		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer);
		::ShowWindow(mHwd, SW_SHOW);
		return 0;
	}
	void WindowClient::ShowWindow(bool isShow)
	{
		if (isShow) {
			SendMessage(mHwd, 0x666, NULL, NULL);
		}
		else {
			SendMessage(mHwd, 0x667, NULL, NULL);
		}
		
	}
	HANDLE WindowClient::GetShareHandle()
	{
		return HANDLE();
	}
	HWND WindowClient::GetWindowId()
	{
		if (mHwd != NULL) {
			return mHwd;
		}
		return 0;
	}
	ID3D11Texture2D * WindowClient::GetWindowBackBuffer()
	{
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer);
		D3D11_TEXTURE2D_DESC textureInfo;
		mBackBuffer->GetDesc(&textureInfo);
		return mBackBuffer;
	}
	ID3D11Texture2D * WindowClient::GetWindowTexture2D()
	{
	
		D3D11_TEXTURE2D_DESC textureInfo;
		mSharedTexture->GetDesc(&textureInfo);
		return mSharedTexture;
	}

	ID3D11DeviceContext * WindowClient::GetDeviceContext()
	{
		return mDeviceContext;
	}
	ID3D11Device * WindowClient::GetDevice()
	{
		return mDevice;
	}
	void WindowClient::UpdateToShareTexture()
	{
		if (mDeviceContext != NULL&&mSwapChain!=NULL) {
			/*ID3D11Texture2D *mBackBuffer1;
			HRESULT hr=mSwapChain->GetBuffer(1, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer1);*/
			//mSwapChain->Present(1, 0);
			mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&mBackBuffer);
			mDeviceContext->CopyResource(mSharedTexture, mBackBuffer);

		}
	}
	void WindowClient::UpdateToWindow()
	{
		if (mDeviceContext != NULL) {
			mDeviceContext->CopyResource(mBackBuffer, mSharedTexture);
			if (mSwapChain != NULL) {
				mSwapChain->Present(1, 0);
			}
		}

	}
	void WindowClient::Present()
	{
		if (mSwapChain != NULL) {
			mSwapChain->Present(0, 0);
		}
	}
	LRESULT  WindowClient::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

	
		PAINTSTRUCT ps;
		HDC hdc;

		switch (msg)
		{
		case WM_PAINT:
			
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_SIZE:

			break;
		case WM_DISPLAYCHANGE:
		case WM_STYLECHANGING:
		case WM_STYLECHANGED:
		case WM_NCLBUTTONDOWN:  //在非工作区按下右键  
		case WM_NCLBUTTONDBLCLK://在非工作区双击右击  
		case WM_NCRBUTTONDOWN:  //在非工作区按下右键  
		case WM_NCRBUTTONDBLCLK://在非工作区双击右击  
			return 0;
		case 0x666:
			::ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);
			return 0;
		case 0x667:
			::ShowWindow(hWnd, SW_HIDE);
			UpdateWindow(hWnd);
			return 0;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		return 0;
		
	}
}