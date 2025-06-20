#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();

	bool load(void* list_indices,UINT size_list);
	UINT getSizeIndexList();
	bool release();

private:
	UINT m_size_list;

	ID3D11Buffer* m_buffer;

	friend class DeviceContext;
};

