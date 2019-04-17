#pragma once
struct  VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

class Engine final
{
public:
	Engine();
	~Engine();

	void Update();
	void Render();

	auto getContext() const -> class Context* { return context; }

private:
	class Context* context;
	class Graphics* graphics;

	class Vertices* vert[8];
	//Shader
	ID3D11VertexShader* vertexShader;
	ID3DBlob* vsBlob;


	//Blob shader가 제대로 컴파일이되었는지 확인해주는함수
	ID3D11PixelShader* pixelShader;
	ID3DBlob* psBlob;

	ID3D11InputLayout* inputLayout;

	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX proj;

	struct TransformData
	{
		D3DXMATRIX WORLD;
		D3DXMATRIX VIEW;
		D3DXMATRIX PROJ;
	}data;

	ID3D11Buffer* cBuffer; // 상수버퍼

	ID3D11RasterizerState* rsState;
};