#pragma once
struct vColor
{
	D3DXVECTOR3 pos;
	D3DXCOLOR color;
};

class Vertices final
{
public:
	Vertices();
	Vertices(Graphics* graphics, int id);
	~Vertices();

	int id;
	vColor* vertices;
	ID3D11Buffer* vertexBuffer;

	/*
	1 3
	0 2
	*/
	float cR;
	float cG;
	float cB;

	float posX;
	float posY;			  
	float size;

	float left, right, top, bottom;

	DWORD timer = 0;
	int vertical;
	int horizontal;

	D3DXMATRIX world;
	D3DXMATRIX view;
	D3DXMATRIX proj;

	struct TransformData
	{
		D3DXMATRIX WORLD;
		D3DXMATRIX VIEW;
		D3DXMATRIX PROJ;
	}data;


	uint* indices = new uint[6]{ 0,1,2,1,3,2 };

	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* cBuffer;

	class Graphics* graphics;

	void createVertBuffer();
	void createConstBuffer();
	void Update();
	void setDir();

	ID3D11RasterizerState* rsState;
	bool isActive = true;
	bool isIntersect(Vertices*& vc);

};