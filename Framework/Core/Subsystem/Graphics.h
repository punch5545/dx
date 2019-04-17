#pragma once
#include "ISubsystem.h"
class Graphics final : public ISubsystem
{
	ID3D11Device* device;			// ����³� : CPU
	ID3D11DeviceContext* dContext;  // �׸��³� : GPU
	IDXGISwapChain* swapChain;		// DirectX Graphic Infrastructure Chain : Double buffer

	ID3D11RenderTargetView* rtv;	// ��ȭ��
	D3D11_VIEWPORT viewport;		// �׷����� ����
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