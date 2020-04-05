#include "InputLayout.h"
#include "GraphicsThrowMacros.h"

InputLayout::InputLayout(Graphics& _gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& _layout, ID3DBlob* _pShaderByteCode)
{
	GFX_THROW_FAILED(GetDevice(_gfx)->CreateInputLayout(
		_layout.data(), (UINT)_layout.size(),
		_pShaderByteCode->GetBufferPointer(),
		_pShaderByteCode->GetBufferSize(),
		&m_pInputLayout
	));
}
void InputLayout::Bind(Graphics& _gfx) noexcept
{
	GetContext(_gfx)->IASetInputLayout(m_pInputLayout.Get());
}