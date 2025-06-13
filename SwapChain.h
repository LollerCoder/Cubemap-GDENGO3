#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DeviceContext;

class SwapChain
{
public:
	bool init(HWND hwnd, UINT width, UINT height);

	bool present(bool vsync);

	bool release();

	ID3D11RenderTargetView* getRTV();
private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;

private:
	friend class DeviceContext;
};

