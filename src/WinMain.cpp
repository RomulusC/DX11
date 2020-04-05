#include "Window.h"
#include <shellapi.h>
#include <sstream>
#include "ExceptionBaseImpl.h"
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
		//throw std::exception();
		return app.Go();		
	}
	catch(const ExceptionBaseImpl& e)
	{
	#ifdef NDEBUG
		MessageBox(nullptr,e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);		
	#endif
	}
	catch (const std::exception& e)
	{
	#ifdef NDEBUG
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	#endif
	}
	catch (...)
	{
	#ifdef NDEBUG
		MessageBox(nullptr, "Details unavailable", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	#else
		std::ostringstream ss;
		ss << "Unknown Exception\nDetails unavailable." << std::endl;
		OutputDebugString(ss.str().c_str());
	#endif
	}	
	return -1;
}