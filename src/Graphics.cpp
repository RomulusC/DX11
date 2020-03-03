#include "Graphics.h"


Graphics::Graphics(HWND hWnd)	
	: m_pDevice(nullptr)
	, m_pDeviceCtx(nullptr)
	, m_pSwapChain(nullptr)
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
	desc.OutputWindow = hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = 0u;

	HRESULT result = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0u,
		nullptr,
		0u,
		D3D11_SDK_VERSION,
		&desc,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceCtx
	);
	if (FAILED(result))
	{
		throw result;
	}
	ID3D11Resource* pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0u/*back buffer*/, __uuidof(ID3D11Resource)/*COM create resource*/, reinterpret_cast<void**>(&pBackBuffer)/*get pointer to backbuffer*/);
	m_pDevice->CreateRenderTargetView(
		pBackBuffer,
		nullptr, // RTV desc
		&m_pRTV
	);
	pBackBuffer->Release();
}

Graphics::~Graphics()
{
	if (m_pRTV != nullptr)
	{
		m_pRTV->Release();
	}
	if (m_pSwapChain != nullptr)
	{
		m_pSwapChain->Release();
	}
	if (m_pDeviceCtx != nullptr)
	{
		m_pDeviceCtx->Release();
	}
	if (m_pDevice != nullptr)
	{
		m_pDevice->Release();
	}
}

void Graphics::EndFrame()
{
	m_pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float _red, float _green, float _blue) noexcept
{
	const float color[] = { _red, _green, _blue };
	m_pDeviceCtx->ClearRenderTargetView(m_pRTV, color);
}
