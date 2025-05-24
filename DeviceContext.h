#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
class SwapChain;
class VertexBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	~DeviceContext();
	bool clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	bool setVertexBuffer(VertexBuffer* vertex_buffer);
	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);
	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	bool release();
	

private:
	ID3D11DeviceContext* m_device_context;
};

