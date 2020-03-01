#include "Window.h"
#include <shellapi.h>
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

		LPWSTR* szArgList;
		int argC;
		szArgList = CommandLineToArgvW(GetCommandLineW(), &argC);
		int xRes = INT_MAX, yRes = INT_MAX;
		for (int i = 0; i < argC; i++)
		{					
			std::wstring str = (szArgList[i]);
			if (str == L"-Res")
			{
				str = szArgList[++i];
				xRes = std::stoi(str);
				str = szArgList[++i];
				yRes = std::stoi(str);

			}
		}
		LocalFree(szArgList);
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