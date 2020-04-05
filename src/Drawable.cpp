#include "Drawable.h"
#include "GraphicsThrowMacros.h"
#include "IndexBuffer.h"

#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& _gfx) const
{
	for (auto& b : m_binds)
	{
		b->Bind(_gfx);
	}
	//_gfx.DrawIndexed(m_pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> _bind)
{
	assert("*Must use AddIndexBuffer to bind index buffer" && typeId(*_bind) != typeid(m_pIndexBuffer));
	m_binds.push_back(std::move(_bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> _ibuf) noexcept
{
	assert("Attempting to add index buffer a second time" && m_pIndexBuffer == nullptr);
	m_pIndexBuffer = _ibuf.get();
	m_binds.push_back(std::move(_ibuf));
}