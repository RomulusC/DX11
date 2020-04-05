#include "PixelShader.h"
#include "GraphicsThrowMacros.h"
#include <d3dcompiler.h>


PixelShader::PixelShader(Graphics& _gfx, const std::wstring& _path)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	GFX_THROW_FAILED(D3DReadFileToBlob(_path.data(), &pBlob));

	GFX_THROW_FAILED(GetDevice(_gfx)->CreatePixelShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), nullptr, &m_pPixelShader ));
}
void PixelShader::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}