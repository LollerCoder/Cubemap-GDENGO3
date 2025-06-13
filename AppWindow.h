#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <vector>
#include "Quads.h"
#include "Cube.h"
#include "Camera.h"
#include <DirectXMath.h>
#include < d3d11.h >
using namespace DirectX;


class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	void updateQuadPosition();

	// Inherited via Window
	void onCreate() override;
	void onUpdate() override;
	void onDestroy() override;

	//singleton
	static AppWindow* get();
	static void initialize();
	static void destroy();

	AppWindow(AppWindow const&) {};
	AppWindow& operator=(AppWindow const&) {};
	static AppWindow* sharedInstance;

private:
	SwapChain* m_swap_chain;
	VertexShader* m_vs;
	PixelShader* m_ps;

	ConstantBuffer* m_cb;


	float m_old_delta;
	float m_new_delta;
	float m_delta_time;

	float m_delta_pos;
	float m_delta_scale;

	Camera camera;

	std::vector<Quads> quadList = {};
	std::vector<Cube> cubeList = {};

	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	

	ID3D11RasterizerState* rasterizerState;
};

