#include "Window.h"
#include <shellapi.h>
#include <sstream>
#include "ExceptionImpl.h"
#include "Application.h"
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

		Application app(xRes, yRes, "DX_3D11");
		return app.Go();		
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