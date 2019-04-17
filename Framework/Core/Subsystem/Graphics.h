#pragma once
#include "ISubsystem.h"
class Graphics final : public ISubsystem
{
	ID3D11Device* device;			// 만드는놈 : CPU
	ID3D11DeviceContext* dContext;  // 그리는놈 : GPU
	IDXGISwapChain* swapChain;		// DirectX Graphic Infrastructure Chain : Double buffer

	ID3D11RenderTargetView* rtv;	// 도화지
	D3D11_VIEWPORT viewport;		// 그려지는 영역
	D3DXCOLOR clearColor;			// 

public:
	Graphics(Context* context);
	~Graphics();

	const bool Initialize() override;

	auto GetDevice() const -> ID3D11Device* { return device; }
	auto GetDeviceContext() const -> ID3D11DeviceContext* { return dContext; }
	auto GetSwapChin() const ->IDXGISwapChain* { return swapChain; }

	void beginScene();
	void endScene();

private:
	void CreateBackBuffer(const uint& width, const uint& height);
};