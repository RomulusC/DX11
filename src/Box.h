#pragma once
#include "Drawable.h"

class Box : public Drawable
{
public:
	Box(Graphics& _gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& _adist,
		std::uniform_real_distribution<float>& _ddist,
		std::uniform_real_distribution<float>& _odist,
		std::uniform_real_distribution<float>& _rdist);
	void Update(float _dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};