#pragma once
#include "Bindable.h"

#include <vector>
class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics& _gfx, const std::vector<unsigned short>& _indices);
	void Bind(Graphics& _gfx) noexcept override;
	UINT GetCount() const noexcept;
protected: 
	UINT m_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
private:
};