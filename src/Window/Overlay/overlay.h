#pragma once
#include "../header.h"
#include <dwmapi.h>
#include <thread>
#include <d3d11.h>
#include <string>
#pragma comment(lib, "d3d11.lib")	

enum InitializeMode : int
{
	WINDOW_TITLE,
	WINDOW_CLASS,
	PROCESS
};

class Overlay
{
private:

	WNDCLASSEXA wc{};
	HWND Hwnd;
	HWND TargetHwnd;
	char Title[32] = "The Overlay";
	char Class[32] = "WND_CLS";
	char TargetTitle[128]{};
	bool CreateOverlay();
	HWND GetTargetWindow(const std::wstring processName);
public:
	bool InitOverlay(const std::wstring targetName, int mode);
	void OverlayLoop();
	void DestroyOverlay();
	void OverlayManager(const char* targetName);
	static void RenderMenuGui();
};

extern bool IsKeyDown(int VK);
extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;