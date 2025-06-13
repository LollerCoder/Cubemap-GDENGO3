#include "AppWindow.h"
#include <Windows.h>
#include "Vector3D.h"
#include "Matrix4x4.h"

AppWindow* AppWindow::sharedInstance;

__declspec(align(16))
struct constant
{
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	unsigned int m_time;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	constant cc;
	cc.m_time = ::GetTickCount();

	m_delta_pos += m_delta_time / 10.0f;

	if (m_delta_pos > 1.f) {
		m_delta_pos = 0;
	}

	m_delta_scale += m_delta_time / .15f;

	Matrix4x4 temp;

	//cc.m_world.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));
	//cc.m_world.setScale(Vector3D::lerp(Vector3D(.5, .5, 0), Vector3D(2, 2, 0), (sin(m_delta_scale) + 1.f) / 2.f));

	//temp.setTranslation(Vector3D::lerp(Vector3D(-2, -2, 0), Vector3D(2, 2, 0), m_delta_pos));
	//cc.m_world *= temp;

	cc.m_world.setScale(Vector3D(1, 1, 1));

	/*temp.setIdentity();
	temp.setRotationZ(m_delta_scale);
	cc.m_world *= temp;*/
	
	/*temp.setIdentity();
	temp.setRotationY(m_delta_scale);
	cc.m_world *= temp;
	
	temp.setIdentity();
	temp.setRotationX(m_delta_scale);
	cc.m_world *= temp;*/


	cc.m_world.setIdentity();
	cc.m_view = camera.GetViewMatrix();
	cc.m_proj = camera.GetProjectionMatrix();
		
		/*setOrthoLH
	(
		(this->getClientWindowRect().right - this->getClientWindowRect().left) / 400.f,
		(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 400.f,
		-4.f,
		4.f
	);*/

	m_cb->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);
}

void AppWindow::onCreate()
{
	//Window::onCreate();
	GraphicsEngine::get()->init();

	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();

	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

	

	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shader_byte_code, &size_shader);
	m_vs = GraphicsEngine::get()->createVertexShader(shader_byte_code, size_shader);

	std::vector<Vector3D> colors;
	std::vector<Vector3D> colors2;
	colors.push_back(Vector3D(1, 0, 0));
	colors.push_back(Vector3D(1,0 , 0));
	colors.push_back(Vector3D(1, 0, 0));
	colors.push_back(Vector3D(1, 0, 0));

	colors2.push_back(Vector3D(1, 0, 0));
	colors2.push_back(Vector3D(1, 0, 0));
	colors2.push_back(Vector3D(1, 0, 0));
	colors2.push_back(Vector3D(1, 0, 0));
	//                         w     h     cx   cy    list
	quadList.push_back(Quads(1.0f, 1.0f, 0.0f, 0.0f,colors, colors2));

	colors.clear();
	colors2.clear();
	colors.push_back(Vector3D(0, 1, 0));
	colors.push_back(Vector3D(0, 1, 0));
	colors.push_back(Vector3D(0, 1, 0));
	colors.push_back(Vector3D(0, 1, 0));

	colors2.push_back(Vector3D(0, 1, 0));
	colors2.push_back(Vector3D(0, 1, 0));
	colors2.push_back(Vector3D(0, 1, 0));
	colors2.push_back(Vector3D(0, 1, 0));
	// //                       w     h     d     cx   cy     cz      list
	cubeList.push_back(Cube(0.3f, 0.3f, 0.3f, 0.f, 0.f, 0.f, colors, colors2));
	/*
	quadList.push_back(Quads(0.4f, 0.2f, -0.6f, -0.3f, 0, 0, colors, colors2));
	colors.clear();
	colors.push_back({ 0,0,1 });
	colors.push_back({ 0,1,1 });
	colors.push_back({ 1,0,1 });
	colors.push_back({ 0,1,0 });
	colors2.clear();
	colors2.push_back({ 0,1,0 });
	colors2.push_back({ 1,1,1 });
	colors2.push_back({ 1,0,0 });
	colors2.push_back({ 0,1,1 });
	quadList.push_back(Quads(0.7f, 0.5f, 0.2f, -0.5f, -0.2f, 0.8f, colors, colors2));
	*/
	
	for (int i = 0; i < quadList.size();i++) {
		quadList[i].createBuffer(&shader_byte_code, &size_shader);
	}
	
	for (int i = 0; i < cubeList.size();i++) {
		cubeList[i].createBuffer(&shader_byte_code, &size_shader);
	}

	GraphicsEngine::get()->releaseCompiledShader();


	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shader_byte_code, &size_shader);
	m_ps = GraphicsEngine::get()->createPixelShader(shader_byte_code, size_shader);

	GraphicsEngine::get()->releaseCompiledShader();

	constant cc;
	cc.m_time = 0;

	m_cb = GraphicsEngine::get()->createConstantBuffer();
	m_cb->load(&cc, sizeof(constant));

	camera.SetPosition(1.5f, 0.1f, -1.0f);
	camera.SetProjectionValues(90.0f, static_cast<float>(rc.right - rc.left) / static_cast<float>(rc.bottom - rc.top), 0.1f, 100.0f);
	camera.SetLookAtPos(XMFLOAT3(0.0f,0.f,0.0f));


	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = rc.right - rc.left;
	depthStencilDesc.Height = rc.bottom - rc.top;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HRESULT hr = GraphicsEngine::get()->m_d3d_device->CreateTexture2D(&depthStencilDesc, NULL, &this->depthStencilBuffer);
	if (FAILED(hr)) //If error occurred
	{
	}
		

	hr = GraphicsEngine::get()->m_d3d_device->CreateDepthStencilView(depthStencilBuffer, NULL, &this->depthStencilView);
	if (FAILED(hr)) //If error occurred
	{
		
		
	}

	/*GraphicsEngine::get()->getImmediateDeviceContext()->m_device_context->OMSetRenderTargets(1, m_swap_chain->getRTV(), this->depthStencilView.Get());*/

	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = GraphicsEngine::get()->m_d3d_device->CreateDepthStencilState(&depthstencildesc, &this->depthStencilState);
	if (FAILED(hr))
	{
	
	}

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = rc.right - rc.left;
	viewport.Height = rc.bottom - rc.top;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//Set the Viewport
	GraphicsEngine::get()->getImmediateDeviceContext()->m_device_context->RSSetViewports(1, &viewport);

	//Create Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	hr = GraphicsEngine::get()->m_d3d_device->CreateRasterizerState(&rasterizerDesc, &this->rasterizerState);
	if (FAILED(hr))
	{
		
		
	}
}

void AppWindow::onUpdate()
{
	//set color here
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain,
		.4, 0.4, 0, 1, this->depthStencilView);
	
	GraphicsEngine::get()->getImmediateDeviceContext()->m_device_context->RSSetState(this->rasterizerState);
	GraphicsEngine::get()->getImmediateDeviceContext()->m_device_context->ClearDepthStencilView(this->depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	GraphicsEngine::get()->getImmediateDeviceContext()->m_device_context->OMSetDepthStencilState(this->depthStencilState, 0);
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	updateQuadPosition();


	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_vs, m_cb);
	GraphicsEngine::get()->getImmediateDeviceContext()->setConstantBuffer(m_ps, m_cb);


	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vs);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_ps);

	
	for (int i = 0; i < quadList.size();i++) {
		quadList[i].draw();
	}
	
	for (int i = 0; i < cubeList.size();i++) {
		cubeList[i].draw();
	}

	m_swap_chain->present(true);

	m_old_delta = m_new_delta;
	m_new_delta = ::GetTickCount();

	m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.f) : 0;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	m_vs->release();
	m_ps->release();
	m_cb->release();
	/*
	for (int i = 0; i < quadList.size();i++) {
		quadList[i].destroy();
	}*/

	for (int i = 0; i < cubeList.size();i++) {
		cubeList[i].destroy();
	}

	GraphicsEngine::get()->release();
}

void AppWindow::initialize()
{
	sharedInstance = new AppWindow();
	sharedInstance->init();
}

void AppWindow::destroy()
{
	if (sharedInstance != NULL) {
		sharedInstance->release();
	}
}

AppWindow* AppWindow::get()
{
	if (!sharedInstance) {
		initialize();
	}

	return sharedInstance;
}