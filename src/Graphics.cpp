#include "Graphics.h"
#include "ExceptionBaseImpl.h"
#include "Core.h"
#include <d3dcompiler.h>
#include <sstream>

#define GFX_THROW_FAILED(_hr) if( FAILED(_hr)){ auto ex = Graphics::HrException(__LINE__, __FILE__, _hr); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex;}
#define GFX_DEVICE_REMOVED_EXCEPT(_hr)  auto ex = Graphics::DeviceRemovedException(__LINE__, __FILE__, _hr); OutputDebugString(ex.what()); DEBUG_BREAK(); throw ex

	Graphics::Graphics(HWND _hWnd, unsigned int _ResWidth, unsigned int _ResHeight)
	: m_pDevice(nullptr)
	, m_pDeviceCtx(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRTV(nullptr)
	, m_xRes(_ResWidth)
	, m_yRes(_ResHeight)
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

	// bind render target
	m_pDeviceCtx->OMSetRenderTargets(1u, m_pRTV.GetAddressOf(), nullptr);

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
}

void Graphics::DrawTestTriangle()
{
	// Set primitive topology to triangle list (groups of 3 vertices)
	m_pDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	struct Vertex
	{
		struct
		{
			float x, y, z;
		}pos;
		struct
		{
			unsigned char r, g, b, a;
		};
	};

	// create vertex buffer (1 2d triangle at center of screen)
	const Vertex vertices[] =
	{
		{ -0.5f, 0.5f, 1.0f, 255,  0,  0,255 },
		{  0.5f, 0.5f, 1.0f,   0,255,  0,255 },
		{  0.5f,-0.5f, 1.0f,   0,  0,255,255 },
		{ -0.5f,-0.5f, 0.7f,  0,   0,  0,255 },
	};
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_FAILED(m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_pDeviceCtx->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	const short indices[] =
	{
		0,1,2,
		2,3,0,
	};
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ib = {};
	ib.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ib.Usage = D3D11_USAGE_DEFAULT;
	ib.CPUAccessFlags = 0u;
	ib.CPUAccessFlags = 0u;
	ib.ByteWidth = sizeof(indices);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	GFX_THROW_FAILED(m_pDevice->CreateBuffer(&ib, &isd, pIndexBuffer.GetAddressOf()));

	m_pDeviceCtx->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// create pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob));
	GFX_THROW_FAILED(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader
	m_pDeviceCtx->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob));
	GFX_THROW_FAILED(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// bind vertex shader
	m_pDeviceCtx->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	// input (vertex) layout (2d position only)
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Color",0,DXGI_FORMAT_R8G8B8A8_UNORM,0,12u,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	GFX_THROW_FAILED(m_pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	));

	// bind vertex layout
	m_pDeviceCtx->IASetInputLayout(pInputLayout.Get());

	m_pDeviceCtx->DrawIndexed((UINT)std::size(indices), 0u, 0);
	//m_pDeviceCtx->Draw((UINT)std::size(vertices), 0u);
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
