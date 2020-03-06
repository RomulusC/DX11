#pragma once

#include "WinDefines.h"
#include "Core.h"
#include "ExceptionBaseImpl.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <memory>
#include <optional>
#include "Graphics.h"


#define CHWND_EXCEPT(hr) Window::Exception ex = Window::Exception(__LINE__, __FILE__, hr); DEBUG_BREAK(); OutputDebugString(ex.what()); throw ex
#define CHWND_THROW_LAST_EXCEPT() Window::Exception ex = Window::Exception(__LINE__, __FILE__, GetLastError()); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex
#define CHWND_NOGFX_EXCEPTION() Window::NoGfxException ex = Window::NoGfxException(__LINE__, __FILE__); DEBUG_BREAK(); OutputDebugString(ex.what()); throw ex
class Window
{
public:
	class Exception : public ExceptionBaseImpl
	{
	public:
		Exception(int _line, const char* _file, HRESULT _hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT _hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT m_hr;
	};
	class NoGfxException : public ExceptionBaseImpl
	{
	public:
		NoGfxException(int _line, const char* _file) noexcept;
		virtual const char* GetType() const noexcept override;
	};
private:
	class WindowClass
	{
		// singleton window
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* s_wndClassName = "DXD11";
		static WindowClass s_wndClass;
		HINSTANCE m_hInst;
	};
public:
	Window(int _width, int _height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void OnResize(HWND hWnd, int x, int y);
public:
	void ChangeTitle(const char* _str);
	static std::optional<int> ProcessMessage();
	Graphics& GetGfx();
public:
	Keyboard m_keyboard;
	Mouse m_mouse;
	int m_width;
	int m_height;
private:
	
	HWND m_hWnd;
	std::unique_ptr<Graphics> m_pGfx;
	
};