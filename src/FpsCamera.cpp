#include "FpsCamera.h"
#include <sstream>

FpsCamera::FpsCamera(DirectX::XMMATRIX _proj, DirectX::XMMATRIX _cameraWorld) noexcept
	: m_moveSpeedConst(50.0f)
	, m_projMatXM(_proj)
	, m_cameraTransformXM(_cameraWorld)
{
	
}
void FpsCamera::Update(float _dt, Keyboard& _keyboard, Mouse& _mouse) noexcept
{
	auto mouseEvent = _mouse.Read();
	if (mouseEvent.has_value())
	{
		std::ostringstream ss;

		switch (mouseEvent->GetType())
		{
		case Mouse::Event::Type::ScrollUp:
		{
			m_moveSpeedConst += 5.0f;
			break;
		}
		case Mouse::Event::Type::ScrollDown:
		{
			m_moveSpeedConst -= 5.0f;
			m_moveSpeedConst = m_moveSpeedConst >= 1.0f ? m_moveSpeedConst : 1.0f;
			break;
		}
		case Mouse::Event::Type::MMClickPress:
		{}
		case Mouse::Event::Type::LPress:
		{}
		case Mouse::Event::Type::RPress:
		{}
		case Mouse::Event::Type::MMClickRelease:
		{}
		case Mouse::Event::Type::LRelease:
		{}
		case Mouse::Event::Type::RRelease:
		{}
		}
	}
	float relativeMove = m_moveSpeedConst * _dt;

	DirectX::XMFLOAT4 relCameraMovement = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (_keyboard.KeyIsPressed(0x57)) // W
	{
		relCameraMovement.z += relativeMove;
	}
	if (_keyboard.KeyIsPressed(0x41)) // A
	{
		relCameraMovement.x += -relativeMove;
	}
	if (_keyboard.KeyIsPressed(0x53)) // S
	{
		relCameraMovement.z += -relativeMove;
	}
	if (_keyboard.KeyIsPressed(0x44)) // D
	{
		relCameraMovement.x += relativeMove;
	}
	if (_keyboard.KeyIsPressed(0x51)) // Q
	{
		m_cameraTransformXM *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(-1.0f));
	}
	if (_keyboard.KeyIsPressed(0x45)) // E
	{
		m_cameraTransformXM *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(1.0f));
	}
	if (_keyboard.KeyIsPressed(0x20)) // SPACE
	{
		relCameraMovement.y += relativeMove;
	}
	if (_keyboard.KeyIsPressed(0x11)) // CTRL
	{
		relCameraMovement.y += -relativeMove;
	}
	if (_keyboard.KeyIsPressed(VK_LEFT))
	{
		m_cameraTransformXM *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(1.0f));
	}
	if (_keyboard.KeyIsPressed(VK_RIGHT))
	{
		m_cameraTransformXM *= DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(-1.0f));
	}
	if (_keyboard.KeyIsPressed(VK_UP))
	{
		m_cameraTransformXM *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(1.0f));
	}
	if (_keyboard.KeyIsPressed(VK_DOWN))
	{
		m_cameraTransformXM *= DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(-1.0f));
	}
	if (_keyboard.KeyIsPressed(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}
	m_cameraTransformXM *= DirectX::XMMatrixTranslation(-relCameraMovement.x, -relCameraMovement.y, -relCameraMovement.z);
	
}

DirectX::XMMATRIX FpsCamera::GetProjMatXM() noexcept
{
	return m_projMatXM;
}

DirectX::XMMATRIX FpsCamera::GetCameraMatXM() noexcept
{
	return m_cameraTransformXM;
}

DirectX::XMMATRIX FpsCamera::GetCameraProjMatXM() noexcept
{
	return m_cameraTransformXM * m_projMatXM;
}
