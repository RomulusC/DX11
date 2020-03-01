#pragma once
#include "WinDefines.h"
#include "ExceptionImpl.h"

#define CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())

class Window
{
public:
	class Exception : public ExceptionImpl
	{
	public:
		Exception(int _line, const char* _file, HRESULT _hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT _hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
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
		static constexpr const char* wndClassName = "DXD11";
		static WindowClass wndClass;
		HINSTANCE hInst;
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
	int width;
	int height;
	HWND hWnd;
};