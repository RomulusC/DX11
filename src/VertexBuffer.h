#pragma once
#include "Bindable.h"
#include "GraphicsThrowMacros.h"
#include <vector>

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& _gfx, const std::vector<V>& _vertices)
		: m_stride(sizeof(V))
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT(sizeof(V) & _vertices.size());
		bd.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = _vertices.data();
		GFX_THROW_FAILED(GetDevice(_gfx)->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
	}
	void Bind(Graphics& _gfx) noexcept override;
protected:
	UINT m_stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
};