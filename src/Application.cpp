#include "Application.h"
#include <sstream>

Application::Application(int _xRes, int _yRes, const char* _name)
	: m_name(std::string(_name))
	, m_window(Window(_xRes, _yRes, _name))
	 
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
			ss << "SCROLLING: +" << std::endl;
			break;
		case Mouse::Event::Type::ScrollDown:
			ss << "SCROLLING: -" << std::endl;
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
}