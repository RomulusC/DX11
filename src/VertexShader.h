#pragma once
#include "Bindable.h"

class VertexShader : public Bindable
{
	VertexShader(Graphics& _gfx, const std::wstring& _path);
	void Bind(Graphics& _gfx) noexcept override;
	ID3DBlob* GetByteCode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> m_byteCodeBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
};