#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
public: 
	Topology(Graphics& _gfx, D3D11_PRIMITIVE_TOPOLOGY _type);
	void Bind(Graphics& _gfx) noexcept override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY m_type;
};