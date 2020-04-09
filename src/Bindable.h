#pragma once
#include "Graphics.h"

class Bindable
{
public:
	virtual void Bind(Graphics& _gfx) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11DeviceContext* GetContext(Graphics& _gfx) noexcept;
	static ID3D11Device* GetDevice(Graphics& _gfx) noexcept;
};