#include "Window.h"
#include "resource.h"
#include <sstream>

Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	: 
	hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(101),IMAGE_ICON, 0, 0, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = WindowClass::wndClassName;
	wc.hIcon = wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, 0));

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}

Window::Window(int _width, int _height, const char* name)
	: width(_width)
	, height(_height)	
{
	RECT desktopSpec = { 0 };
	GetWindowRect(GetDesktopWindow(), &desktopSpec);
	RECT appSpec = { 0 };
	
	if (desktopSpec.right >= width && desktopSpec.bottom >= height)
	{
		appSpec.right = width;
		appSpec.bottom = height;
	}
	else
	{
		appSpec.right = 960;
		appSpec.bottom = 540;
	}
	long int winConfig = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	BOOL result = AdjustWindowRect(&appSpec, winConfig, FALSE);
	if (result < 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
	hWnd = CreateWindowExA
	(
		0, WindowClass::GetName(), name,
		winConfig,
		appSpec.left + (desktopSpec.right / 2) - (appSpec.right / 2) /* x axis pos of corner */, (desktopSpec.bottom / 2) - (int)(appSpec.bottom * 0.75f) /* y axis pos of corner*/,
		appSpec.right - (appSpec.left*2) /* total pixels left to right */, appSpec.bottom - appSpec.top /* total pixels top to bottom */,
		nullptr, nullptr,  WindowClass::GetInstance(), this
	);
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}
	ShowWindow(hWnd, SW_SHOW);

}

Window::~Window()
{
	DestroyWindow(hWnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::Exception::Exception(int _line, const char* _file, HRESULT _hr) noexcept
	:ExceptionImpl(_line, _file)
	,hr(_hr)
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "!" << std::endl
		<< "[Error Code]: " << GetErrorCode() << std::endl
		<< "[Description]: " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const noexcept
{
	return "HWND Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT _hr) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, _hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0)
	{
		return "Unidentified error code.";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}