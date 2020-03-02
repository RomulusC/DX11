#include "Window.h"
#include <shellapi.h>
#include <string>
#include <sstream>
#include "ExceptionImpl.h"
#include "ArgumentList.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow)
{
	try
	{
		int xRes = INT_MAX, yRes = INT_MAX;
		const auto resArgs = ArgumentList::Instance().FindArguments(ArgumentList::ArgCode::Resolution);
		if (resArgs->size() == 2)
		{
			xRes = std::stoi((resArgs)->at(0));
			yRes = std::stoi(resArgs->at(1));
		}

		Window window = Window(xRes, yRes, "DX3D11");
		// message pump
		MSG msg;
		BOOL gResult;
		window.m_keyboard.EnableAutoRepeat();
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			auto key = window.m_keyboard.ReadKey();
			if (key.has_value())
			{
				std::ostringstream ss;
				OutputDebugString(ss.str().c_str());
			}
			auto key1 = window.m_mouse.Read();
			if (key1.has_value())
			{
				std::ostringstream ss;
				switch (key1->GetType())
				{
				case Mouse::Event::Type::Move:
				{
					std::ostringstream sd;
					sd << "DX3D11 (" << key1->GetPosX() << "," << key1->GetPosY() << ")";
					window.ChangeTitle(sd.str().c_str());
					break;
				}
				case Mouse::Event::Type::ScrollUp:
					ss << "+" << "" << std::endl;
					break;
				case Mouse::Event::Type::ScrollDown:
					ss << "-"<<"" << std::endl;
					break;
				default:
					ss <<"Left:"<< key1->IsLeftPressed() <<"Right:"<< key1->IsRightPressed()<< "Middle:"<<key1->IsMMClickPressed() << std::endl;
					break;
				}				
				OutputDebugString(ss.str().c_str());
			}			
		}		
		if (gResult == -1)
		{
			return -1;
		}
		else
		{
			return static_cast<int>(msg.wParam);
		}
	}
	catch(ExceptionImpl& e)
	{
		MessageBox(nullptr,e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);		
	}
	catch (std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, "Details unavailable", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}	
	return -1;	
}