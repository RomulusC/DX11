#include "Graphics.h"
#include "ExceptionBaseImpl.h"
#include "Core.h"
#include <sstream>

#define GFX_THROW_FAILED(_hr) if( FAILED(_hr)){ auto ex = Graphics::HrException(__LINE__, __FILE__, _hr); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex;}
#define GFX_DEVICE_REMOVED_EXCEPT(_hr)  auto ex = Graphics::DeviceRemovedException(__LINE__, __FILE__, _hr); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex

	Graphics::Graphics(HWND _hWnd)	
	: m_pDevice(nullptr)
	, m_pDeviceCtx(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRTV(nullptr)
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0; // no need for refresh rate config, using minimized (for now)
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // if interlaced
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0; // Anti Aliasing (none)
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1; // one front and one back buffer for the swap chain
	desc.OutputWindow = _hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0u;

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0u,
		D3D11_SDK_VERSION,
		&desc,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceCtx
	));
	
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	GFX_THROW_FAILED(m_pSwapChain->GetBuffer(
		0u/*back buffer*/,
		__uuidof(ID3D11Resource)/*COM create resource*/,
		&pBackBuffer/*get pointer to backbuffer*/
	));

	GFX_THROW_FAILED(m_pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr, // RTV desc
		&m_pRTV
	));
}

void Graphics::EndFrame()
{
	HRESULT hr;
	if (FAILED(hr = m_pSwapChain->Present(1u, 0u)))
	{
		GFX_DEVICE_REMOVED_EXCEPT( m_pDevice->GetDeviceRemovedReason());
	}
	else
	{
		GFX_THROW_FAILED(hr);
	}
}

void Graphics::ClearBuffer(float _red, float _green, float _blue, float _alpha) noexcept
{
	const float color[] = { _red, _green, _blue, _alpha };
	m_pDeviceCtx->ClearRenderTargetView(m_pRTV.Get(), color);
}

Graphics::HrException::HrException(int _line, const char* _file, HRESULT _hr) noexcept
	: ExceptionBaseImpl(_line, _file)
	, m_hr(_hr)
{}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << "!" << std::endl
		<< "[Error Code]: " << GetErrorCode() << " 0x" << std::hex << GetErrorCode() << std::endl
		<< "[Description]: " << GetErrorString() << std::endl		
		<< GetOriginString() << std::endl;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Graphics Exception!";
}

std::string Graphics::HrException::TranslateErrorCode(HRESULT _hr) noexcept
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

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return m_hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return TranslateErrorCode(m_hr);
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Device Removed Exception";
}
