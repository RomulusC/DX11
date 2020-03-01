#include "Window.h"
#include <shellapi.h>
#include <string>

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
		const auto resArgs = ArgumentList::Instance().FindArguments(ArgumentList::RESOLUTION);
		if (resArgs->size() == 2)
		{
			xRes = std::stoi((resArgs)->at(0));
			yRes = std::stoi(resArgs->at(1));
		}

		Window window = Window(xRes, yRes, "DX3D11");
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