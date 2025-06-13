#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
class SwapChain;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	~DeviceContext();
	bool clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha,ID3D11DepthStencilView* depthStencilView);
	bool setVertexBuffer(VertexBuffer* vertex_buffer);
	bool setIndexBuffer(IndexBuffer* index_buffer);

	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);
	void setVertexShader(VertexShader* vertex_shader);
	void setPixelShader(PixelShader* pixel_shader);

	void setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer);
	void setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer);

	bool release();
	ID3D11DeviceContext* m_device_context;

private:
	

	friend class ConstantBuffer;
};

