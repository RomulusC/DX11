#pragma once
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	void EndFrame();
	void ClearBuffer(float _red, float _green, float _blue) noexcept;
private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceCtx;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRTV;
};