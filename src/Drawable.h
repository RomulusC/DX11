#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

class Bindable;

class Drawable 
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw(Graphics& _gfx) const; /*(noexcept (N_DEBUG))*/

	virtual void Update(float _dt) noexcept = 0;
	void AddBind(std::unique_ptr<Bindable> _bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> _ibuf) noexcept;
	virtual ~Drawable() = default;

private:
	const IndexBuffer* m_pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;
};