#include "Graphics.h"
#include "ExceptionBaseImpl.h"
#include "Core.h"
#include "GraphicsThrowMacros.h"

#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <sstream>

namespace DX = DirectX;

	Graphics::Graphics(HWND _hWnd, unsigned int _ResWidth, unsigned int _ResHeight)
	: m_pDevice(nullptr)
	, m_pDeviceCtx(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRTV(nullptr)
	, m_xRes(_ResWidth)
	, m_yRes(_ResHeight)
	, m_fov(70.0f)
	, m_fpsCamera(FpsCamera(DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(m_fov), (float)_ResWidth/ (float)_ResHeight, .05f, 1000.0f), DirectX::XMMatrixIdentity()))
{
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width = (UINT)m_xRes;
	desc.BufferDesc.Height = (UINT)m_yRes;
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

	UINT debugLayerEnum = 0u;
#ifndef FINAL
	debugLayerEnum |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		debugLayerEnum,
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


	

	D3D11_DEPTH_STENCIL_DESC depthDesc = {};
	depthDesc.DepthEnable = TRUE;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	m_pDevice->CreateDepthStencilState(&depthDesc, &pDSState);
	m_pDeviceCtx->OMSetDepthStencilState(pDSState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = (UINT)m_xRes;
	descDepth.Height = (UINT)m_yRes;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	GFX_THROW_FAILED(m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	GFX_THROW_FAILED(m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV));

	m_pDeviceCtx->OMSetRenderTargets(1u, m_pRTV.GetAddressOf(), m_pDSV.Get());

	// configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_xRes;
	vp.Height = (FLOAT)m_yRes;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pDeviceCtx->RSSetViewports(1u, &vp);

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

void Graphics::ClearBuffer(float _red, float _green, float _blue) noexcept
{
	const float color[] = { _red, _green, _blue, 0.0f};
	m_pDeviceCtx->ClearRenderTargetView(m_pRTV.Get(), color);
	m_pDeviceCtx->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
void Graphics::DrawIndexed(UINT count)
{
	m_pDeviceCtx->DrawIndexed(count, 0u, 0u);
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
