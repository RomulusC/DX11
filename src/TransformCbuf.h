#pragma once
#include "ConstantBuffers.h"
#include "Drawable.h"

#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
public:
	TransformCbuf(Graphics& _gfx, const Drawable& _parent);
private:
	VertexConstantBuffer<DirectX::XMMATRIX> m_vcbuf;
	const Drawable& m_parent;
};