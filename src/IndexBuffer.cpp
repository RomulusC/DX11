#include "IndexBuffer.h"
#include "GraphicsThrowMacros.h"

#include <vector>

IndexBuffer::IndexBuffer(Graphics& _gfx, const std::vector<unsigned short>& _indices)
	: m_count((UINT)_indices.size())
{
	D3D11_BUFFER_DESC idb = {};
	idb.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idb.Usage = D3D11_USAGE_DEFAULT;
	idb.CPUAccessFlags = 0u; // is it?
	idb.MiscFlags = 0u;
	idb.ByteWidth = UINT(m_count * sizeof(unsigned short));
	idb.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = _indices.data();
	GFX_THROW_FAILED(GetDevice(_gfx)->CreateBuffer(&idb, &isd, &m_pIndexBuffer));
}
void IndexBuffer::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() const noexcept
{
	return m_count;
}