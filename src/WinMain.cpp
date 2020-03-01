#include "Window.h"
#include <string>
#include "ExceptionImpl.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR	  lpCmdLine,
	int		  nCmdShow)
{
	try
	{
		Window window = Window(2560 / 2, 1440 / 2, "DX3D11");
		// message pump
		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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