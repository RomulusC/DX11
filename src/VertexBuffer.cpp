#include "VertexBuffer.h"
#include "GraphicsThrowMacros.h"

void VertexBuffer::Bind(Graphics& _gfx)  noexcept
{
	const UINT offset = 0u;
	GetContext(_gfx)->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &offset);
}