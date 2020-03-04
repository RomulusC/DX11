#pragma once
#include <d3d11.h>
#include "ExceptionBaseImpl.h"

class Graphics
{
public:	
	
	class HrException : public ExceptionBaseImpl
	{
	public:
		using ExceptionBaseImpl::ExceptionBaseImpl;
		HrException(int _line, const char* _file, HRESULT _hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT _hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT m_hr;
	};
	class DeviceRemovedException : public HrException
	{
	public:
		using HrException::HrException;
		const char* GetType() const noexcept override;
	};

	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float _red, float _green, float _blue, float _alpha) noexcept;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceCtx;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRTV;
};