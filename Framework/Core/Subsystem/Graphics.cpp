#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics(Context * context)
	: ISubsystem(context)
	, device(nullptr)
	, dContext(nullptr)
	, swapChain(nullptr)
	, rtv(nullptr)
	, clearColor(0xff555566)
{
}

Graphics::~Graphics()
{
}

const bool Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC desc; // description
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferDesc.Width					= 0;
	desc.BufferDesc.Height					= 0;
	desc.BufferDesc.RefreshRate.Numerator   = 60; // Framerate
	desc.BufferDesc.RefreshRate.Denominator = 1;  // per second
	desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM; // unsigned normalize 정규화된 양수 (0~255 -> 0.0f~1.0f)
	desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

	desc.BufferCount						= 1;
	desc.SampleDesc.Count					= 1; // 샘플링할 횟수
	desc.SampleDesc.Quality					= 0; // AA 퀄리티
	// Super Sample Anti Aliasing : 텍스쳐 픽셀을 늘렸다가 줄임
	// Multi Sample Anti Aliasing : 외곽선 픽셀만 건들임

	desc.OutputWindow						= Settings::Get().getWindowHandle(); //TODO : desc.OutputWindow
	desc.Windowed							= true;
	desc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD; // 이전 화면 지움
	desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;

	D3D_FEATURE_LEVEL featLevels[] = // 기능 수준
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE, // PC 성능 최대치를 끌어올림, 지원안하면 못씀
		//hardware : 빠름 , software : 느림
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, // bgra지원
		featLevels,
		1,
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&dContext
	);

	assert(SUCCEEDED(hr));

	CreateBackBuffer
	(
		static_cast<uint>( Settings::Get().getWidth() ),
		static_cast<uint>( Settings::Get().getWidth() )
	);
	return true;
}

void Graphics::beginScene() // 그리는놈
{
	dContext->RSSetViewports(1, &viewport);
	dContext->OMSetRenderTargets(1, &rtv, nullptr);
	dContext->ClearRenderTargetView(rtv, clearColor);
}

void Graphics::endScene() // 보여주는놈
{
	HRESULT hr = swapChain->Present(1, 0);
	
	assert(SUCCEEDED(hr));
}

void Graphics::CreateBackBuffer(const uint & width, const uint & height)
{
	HRESULT hr = swapChain->ResizeBuffers
	(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0
	);

	assert(SUCCEEDED(hr));

	ID3D11Texture2D* backBuffer = nullptr; // CPU, GPU -> ...View
	hr = swapChain->GetBuffer
	(
		0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&backBuffer)
	);
	// UUID : Usualy  Unique Identifier : Any OS     고유식별자
	// GUID : Globaly Unique Identifier : MS         16byte(128bit)
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		backBuffer,
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width    = static_cast<float>(width);
	viewport.Height   = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	SAFE_RELEASE(backBuffer);
}
