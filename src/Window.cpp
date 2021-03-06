#include "Window.h"
#include <Windowsx.h>
#include "../res/ICON_1.h"
#include <sstream>

Window::WindowClass Window::WindowClass::s_wndClass;

Window::WindowClass::WindowClass() noexcept
	: 
	m_hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON, 0, 0, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = WindowClass::s_wndClassName;
	wc.hIcon = wc.hIcon = static_cast<HICON>(LoadImage(m_hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 0, 0, 0));

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(s_wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return s_wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return s_wndClass.m_hInst;
}

Window::Window(int _width, int _height, const char* name)
	: m_width(_width)
	, m_height(_height)	
{
	RECT desktopSpec = { 0 };
	GetWindowRect(GetDesktopWindow(), &desktopSpec);
	RECT appSpec = { 0 };
	
	if (desktopSpec.right >= _width && desktopSpec.bottom >= m_height && m_width >= 600 && m_height >= 400)
	{
		appSpec.right = m_width;
		appSpec.bottom = m_height;
	}
	else
	{
		appSpec.right = 1280;
		appSpec.bottom = 720;
		m_width = appSpec.right;
		m_height = appSpec.bottom;
	}
	long int winConfig = WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
	BOOL result = AdjustWindowRect(&appSpec, winConfig, FALSE);
	if (result == 0)
	{
		CHWND_THROW_LAST_EXCEPT();
	}
	int x = appSpec.left + (desktopSpec.right - (appSpec.right - appSpec.left))/2;
	x = x > appSpec.left ? x : appSpec.left;
	int y = (desktopSpec.bottom - (appSpec.bottom - appSpec.top))/4;
	y = y > 0 ? y : 0;
	m_hWnd = CreateWindowExA
	(
		0, WindowClass::GetName(), name,
		winConfig,
		x,y,
		appSpec.right - (appSpec.left*2) /* total pixels left to right */, appSpec.bottom - appSpec.top /* total pixels top to bottom */,
		nullptr, nullptr,  WindowClass::GetInstance(), this
	);

	if (m_hWnd == nullptr)
	{
		CHWND_THROW_LAST_EXCEPT();
	}	

	m_pGfx = std::make_unique<Graphics>(m_hWnd, m_width, m_height);
	ShowWindow(m_hWnd, SW_SHOW);
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
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

LRESULT WINAPI Window::HandleMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// use thread pool for long message handling callbacks.
	switch (uMsg)
	{

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_SIZE:
	{
		OnResize(hWnd, LOWORD(lParam), HIWORD(lParam));
		return 0;
	}
	case WM_KILLFOCUS:
	{
		m_keyboard.ClearState();
		return 0;
	}
	/* KEY STROKES */	
	case WM_SYSCHAR:
	{		
		break;
	}	
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{		
		if (lParam & 0x40000000 || m_keyboard.IsAutoRepeatEnabled()) // 30th bit represents if key pressed before
		{
			m_keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		m_keyboard.OnKeyRelease(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_CHAR:
	{
		m_keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	}
	case WM_MOUSEMOVE:
	{
		POINTS pts = MAKEPOINTS(lParam);
		if (pts.x >= 0 && pts.x < m_width && pts.y >= 0 && pts.y < m_height)
		{
			if (!m_mouse.IsMouseInWindow())
			{
				SetCapture(hWnd);
				m_mouse.OnMouseEnterWindow();
			}
			m_mouse.OnMouseMove(pts.x, pts.y);
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				m_mouse.OnMouseMove(pts.x, pts.y);
			}
			else
			{
				m_mouse.OnMouseLeaveWindow();
				ReleaseCapture();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		m_mouse.OnLeftPressed(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_LBUTTONUP:
	{
		m_mouse.OnLeftReleased(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		m_mouse.OnRightPressed(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_RBUTTONUP:
	{
		m_mouse.OnRightReleased(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_MBUTTONDOWN:
	{
		m_mouse.OnMMPressed(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_MBUTTONUP:
	{
		m_mouse.OnMMReleased(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	}
	case WM_MOUSEWHEEL:
		m_mouse.OnWheelUpdate(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_WHEEL_DELTA_WPARAM(wParam));
		break;	
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
void Window::OnResize(HWND hWnd, int x, int y)
{	
	/*
	std::stringstream ss;
	ss << "OnResize() x:" << x<< " y:" << y << std::endl;
	MessageBox(nullptr, ss.str().c_str(), "Who cares", MB_OK | MB_ICONEXCLAMATION);	
	*/
}

void Window::ChangeTitle(const char* _str)
{
	if (SetWindowText(m_hWnd, _str) == 0)
	{
		CHWND_THROW_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessage()
{
	// message pump
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return static_cast<int>(msg.wParam);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return std::optional<int>();
}

Graphics& Window::Gfx()
{
	if (!m_pGfx)
	{
		CHWND_NOGFX_EXCEPTION();
	}
	return *m_pGfx;
}

Window::Exception::Exception(int _line, const char* _file, HRESULT _hr) noexcept
	:ExceptionBaseImpl(_line, _file)
	,m_hr(_hr)
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
	return m_hr;
}

std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(m_hr);
}

Window::NoGfxException::NoGfxException(int _line, const char* _file) noexcept
	: ExceptionBaseImpl(_line, _file)
{}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "No Gfx Exception";
}
