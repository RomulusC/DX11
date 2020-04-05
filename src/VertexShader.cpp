#include "VertexShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>

VertexShader::VertexShader(Graphics& _gfx, const std::wstring& _path)
{
	GFX_THROW_FAILED(D3DReadFileToBlob(_path.c_str(), &m_byteCodeBlob));
	GFX_THROW_FAILED(GetDevice(_gfx)->CreateVertexShader(
		m_byteCodeBlob->GetBufferPointer(),
		m_byteCodeBlob->GetBufferSize(),
		nullptr,
		&m_pVertexShader
	));
}

void VertexShader::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept
{
	return m_byteCodeBlob.Get();
}