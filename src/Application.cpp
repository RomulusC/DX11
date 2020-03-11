#include "Application.h"
#include <sstream>

Application::Application(int _xRes, int _yRes, const char* _name)
	: m_name(std::string(_name))
	, m_window(Window(_xRes, _yRes, _name))
	, m_timer(Timer())
	, m_cameraTransform(DirectX::XMMATRIX(DirectX::XMMatrixIdentity()* DirectX::XMMatrixTranslation(0.0f,0.0f,5.0f)))
	 
{
	m_window.m_keyboard.EnableAutoRepeat();
}

int Application::Go()
{
	while (true)
	{
		if (const std::optional<int> ecode = m_window.ProcessMessage())
		{
			return ecode.value();
		}
		DoFrame();
	}
	return 0;
}

void Application::DoFrame()
{
	

	if (m_window.m_keyboard.KeyIsPressed(0x57)) // W
	{
		DirectX::XMFLOAT4 vec = { 0.0f, 0.0f, -1.0f, 0.0f };
		m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), m_cameraTransform.r[2]));
	}
	if (m_window.m_keyboard.KeyIsPressed(0x41)) // A
	{
		DirectX::XMFLOAT4 vec = { 1.0f, 0.0f, 0.0f, 0.0f };
		m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), m_cameraTransform.r[0]));
	}
	if (m_window.m_keyboard.KeyIsPressed(0x53)) // S
	{
		DirectX::XMFLOAT4 vec = { 0.0f, 0.0f, 1.0f, 0.0f };
		m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), m_cameraTransform.r[2]));
	}
	if (m_window.m_keyboard.KeyIsPressed(0x44)) // D
	{
		DirectX::XMFLOAT4 vec = { -1.0f, 0.0f, 0.0f, 0.0f };
		m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), m_cameraTransform.r[0]));
	}
	if (m_window.m_keyboard.KeyIsPressed(0x51)) // Q
	{
		m_cameraTransform *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(1.0f));
	}
	if (m_window.m_keyboard.KeyIsPressed(0x45)) // E
	{
		m_cameraTransform *= DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(-1.0f)); /*need cross product of x and z to get up vector*/
	}// more here: https://gamedev.stackexchange.com/questions/90208/how-to-calculate-a-direction-vector-for-camera
	if (m_window.m_keyboard.KeyIsPressed(0x20)) // SPACE
	{
		DirectX::XMFLOAT4 vec = { 0.0f, -1.0f, 0.0f, 0.0f };
		m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), m_cameraTransform.r[1]));

	}
	if (m_window.m_keyboard.KeyIsPressed(0x11)) // CTRL
	{
		DirectX::XMFLOAT4 vec = { 0.0f, 1.0f, 0.0f, 0.0f };
		m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), m_cameraTransform.r[1])); 
	}

	auto mouseEvent = m_window.m_mouse.Read();
	if (mouseEvent.has_value())
	{
		std::ostringstream ss;
		switch (mouseEvent->GetType())
		{		
		case Mouse::Event::Type::ScrollUp:
		{
			
			DirectX::XMFLOAT4 vec = { 1.0f, 0.0f, 0.0f, 0.0f };
			m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), DirectX::XMMatrixTranspose(m_cameraTransform).r[0]));
			break;
		}
		case Mouse::Event::Type::ScrollDown:
		{
			DirectX::XMFLOAT4 vec = { -1.0f, 0.0f, 0.0f, 0.0f };
			m_cameraTransform.r[3] = DirectX::XMVectorAdd(m_cameraTransform.r[3], DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&vec), DirectX::XMMatrixTranspose(m_cameraTransform).r[0]));
			break;
		}
		case Mouse::Event::Type::MMClickPress:
		case Mouse::Event::Type::LPress:
		case Mouse::Event::Type::RPress:
		case Mouse::Event::Type::MMClickRelease:
		case Mouse::Event::Type::LRelease:
		case Mouse::Event::Type::RRelease:
			ss << "Left:" << mouseEvent->IsLeftPressed() << " Middle:" << mouseEvent->IsMMClickPressed() << " Right:" << mouseEvent->IsRightPressed()  << std::endl;
			break;
		}
		OutputDebugString(ss.str().c_str());
	}	



	std::ostringstream ss;
	ss << m_window.m_mouse.GetPosX() << " " << m_window.m_mouse.GetPosY();
	float x = m_window.m_mouse.GetPosX() / ((float)m_window.m_width / 2.0f) - 1.0f;
	float y = -m_window.m_mouse.GetPosY() / ((float)m_window.m_height / 2.0f) + 1.0f;
	ss << " " << x << " " << y;
	m_window.ChangeTitle(ss.str().c_str());

	
	m_window.GetGfx().ClearBuffer(.28f, .6f, 1.0f);
	
	// Draw things
	m_window.GetGfx().DrawTestTriangle(m_timer.Peak()*5,x,y, m_cameraTransform);
	m_window.GetGfx().DrawTestTriangle(0, 0, 0, m_cameraTransform);

	m_window.GetGfx().EndFrame(); // Calls present.
}