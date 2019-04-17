#include "stdafx.h"
#include "Engine.h"
#include "Vertices.h"

Engine::Engine()
{
	context = new Context();
	graphics = context->RegisterSubSystem<Graphics>();

	context->Initialize();

	for (int i = 0; i < 8; i++) {
		if (i == 0) {
			vert[i] = new Vertices(graphics, 1);
		}
		else {
			vert[i] = new Vertices(graphics, 0);
		}
		vert[i]->createVertBuffer();
	}

	//Create Vertex Shader
	{
		HRESULT hr = D3DX11CompileFromFileA
		(
			"Color.hlsl",
			nullptr,
			nullptr,
			"VS",
			"vs_5_0", // Shader Model Version
			0,
			0,
			nullptr,
			&vsBlob,
			nullptr,
			nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreateVertexShader
		(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			&vertexShader
		);
		assert(SUCCEEDED(hr));
	}

	//Create Pixel Shader
	{
		HRESULT hr = D3DX11CompileFromFileA
		(
			"Color.hlsl",
			nullptr,
			nullptr,
			"PS",
			"ps_5_0", // Shader Model Version
			0,
			0,
			nullptr,
			&psBlob,
			nullptr,
			nullptr
		);
		assert(SUCCEEDED(hr));

		hr = graphics->GetDevice()->CreatePixelShader
		(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&pixelShader
		);
		assert(SUCCEEDED(hr));
	}
	//Create Input Layout
	{
		D3D11_INPUT_ELEMENT_DESC layoutDescs[] = // Position, Color
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		HRESULT hr = graphics->GetDevice()->CreateInputLayout
		(
			layoutDescs,
			2,
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			&inputLayout
		);
		assert(SUCCEEDED(hr));
	}

	
	{
		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&proj);

		D3DXMatrixLookAtLH
		(
			&view,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 1),
			&D3DXVECTOR3(0, 1, 0)
		);

		D3DXMatrixOrthoLH
		(
			&proj,
			Settings::Get().getWidth(),
			Settings::Get().getHeight(),
			0.0f,
			1.0f
		);
	}

	//Create Constant Buffer
	//{
	//	D3D11_BUFFER_DESC desc;
	//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	//	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU : write, GPU : read
	//	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//	desc.ByteWidth = sizeof(TransformData);
	//	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//	HRESULT hr = graphics->GetDevice()->CreateBuffer
	//	(
	//		&desc,
	//		nullptr,
	//		&cBuffer
	//	);
	//	assert(SUCCEEDED(hr));
	//}
	for (int i = 0; i < 8; i++) {
		vert[i]->createConstBuffer();
	}

}

Engine::~Engine()
{
	//SAFE_RELEASE(rsState);
	//SAFE_RELEASE(cBuffer);
	SAFE_RELEASE(inputLayout);
	SAFE_RELEASE(psBlob);
	SAFE_RELEASE(vsBlob);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(vertexShader);
	//SAFE_RELEASE(vertexBuffer);
	//SAFE_RELEASE(indexBuffer);
	//
	//SAFE_DELETE_ARR(indices);
	//SAFE_DELETE_ARR(vertices);
	SAFE_DELETE(context);
	for (int i = 7; i > -1; i--)
	{	
		SAFE_DELETE(vert[i]);
	}

}

void Engine::Update()
{
	for (int i = 0; i < 8; i++) {
		vert[i]->Update();
		if (i != 0) {
			if (vert[i]->isIntersect(vert[0])) {
				for (int j = 0; j < 4; j++)
					vert[0]->vertices[j].color = D3DXCOLOR(vert[i]->cR, vert[i]->cG, vert[i]->cB, 1.0f);
				vert[0]->size += 5.0f;
				vert[i]->size == 0.0f;
				vert[i]->posX == 0.0f;
				vert[i]->posY == 0.0f;
				vert[i]->vertices[0].color = { 0,0,0,0 };
				vert[i]->vertices[1].color = { 0,0,0,0 };
				vert[i]->vertices[2].color = { 0,0,0,0 };
				vert[i]->vertices[3].color = { 0,0,0,0 };
			}
		}
	}


}

void Engine::Render()
{
	uint stride = sizeof(VertexColor); // 보폭
	uint offset = 0;				   // 시작점

	for (int i = 0; i < 8; i++) {
		vert[i]->createVertBuffer();
	}

	graphics->beginScene();
	{
		for (int i = 7; i > -1; i--)
		{
			graphics->GetDeviceContext()
				->IASetVertexBuffers(0, 1, &vert[i]->vertexBuffer, &stride, &offset);
			graphics->GetDeviceContext()->IASetInputLayout(inputLayout);
			graphics->GetDeviceContext()->IASetIndexBuffer(vert[i]->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			graphics->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			graphics->GetDeviceContext()->VSSetShader(vertexShader, nullptr, 0);
			graphics->GetDeviceContext()->VSSetConstantBuffers(0, 1, &vert[i]->cBuffer);
			graphics->GetDeviceContext()->RSSetState(vert[i]->rsState);
			graphics->GetDeviceContext()->PSSetShader(pixelShader, nullptr, 0);

			graphics->GetDeviceContext()->DrawIndexed(6, 0, 0);
		}
		
	}
	graphics->endScene();
}



// VS 정점 공간변환
// Local - World - View - Projection (3d -> 2d)
// 4x4 Matrix 

/*
 동차 : x y z w
 w == 0 : 방향
 else   : 위치

 0 0 0 0
 0 0 0 0
 0 0 0 0
 0 0 0 0

 1 0 0 0
 0 1 0 0 
 0 0 1 0
 0 0 0 1

 스케일 * 자전 * 이동 * 공전 * 부모
 S R T
*/