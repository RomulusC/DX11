#pragma once
#include <Windows.h>
#include "Keyboard.h"
#include "Mouse.h"
#include <DirectXMath.h>
class FpsCamera
{
public:
	FpsCamera(DirectX::XMMATRIX _proj, DirectX::XMMATRIX _cameraWorld) noexcept;
	void Update(float _dt, Keyboard& _keyboard, Mouse& _mouse) noexcept;
	DirectX::XMMATRIX GetProjMatXM() noexcept;
	DirectX::XMMATRIX GetCameraMatXM() noexcept;
	DirectX::XMMATRIX GetCameraProjMatXM() noexcept;
private:
	float m_moveSpeedConst;
	DirectX::XMMATRIX m_projMatXM;
	DirectX::XMMATRIX m_cameraTransformXM;
};