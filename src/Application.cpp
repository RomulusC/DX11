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
	// Input
	auto keyEvent = m_window.m_keyboard.ReadKey();
	if (keyEvent.has_value() && (keyEvent->IsPressed()||keyEvent->IsReleased()))
	{
		std::ostringstream ss;	
		
		ss << std::to_string(keyEvent->GetCode()) << std::endl;
		OutputDebugString(ss.str().c_str());
	}
	auto mouseEvent = m_window.m_mouse.Read();
	if (mouseEvent.has_value())
	{
		std::ostringstream ss;
		switch (mouseEvent->GetType())
		{		
		case Mouse::Event::Type::ScrollUp:
			
			m_cameraTransform *= DirectX::XMMatrixTranslation(0.0f,0.0f,0.25f);
			
			//m_window.GetGfx().m_fov += 1.0f;
			//ss << "SCROLLING: +"<< m_window.GetGfx().m_fov << std::endl;
			break;
		case Mouse::Event::Type::ScrollDown:
			m_cameraTransform *= DirectX::XMMatrixTranslation(0.0f, 0.0f, -0.25f);

			//m_window.GetGfx().m_fov -= 1.0f;
			//ss << "SCROLLING: -" << m_window.GetGfx().m_fov << std::endl;

			break;
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
	m_window.GetGfx().DrawTestTriangle(-m_timer.Peak(), -x, -y, m_cameraTransform);

	m_window.GetGfx().EndFrame(); // Calls present.
}