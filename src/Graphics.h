#pragma once
#include <d3d11.h>
#include "ExceptionBaseImpl.h"
#include <wrl.h>

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

	Graphics(HWND hWnd, unsigned int _ResWidth, unsigned int _ResHeight);
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void ClearBuffer(float _red, float _green, float _blue) noexcept;
	void DrawTestTriangle(float _angle, float _mXPos, float _mYPos);
	void EndFrame();
	float m_fov;
	unsigned int m_xRes, m_yRes;
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice; // Allocating things (creates)
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceCtx; // Config pipeline, issuing commands 
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV;
	// when using get of operator (&), it destructs (by calling release) and then writes the address
	// if you wish to obtain ComPtr obj as a reference, use .GetAddressOf()
};