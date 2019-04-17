#include "stdafx.h"
#include "Vertices.h"

std::random_device rd;
std::mt19937 randomEngine(rd());

Vertices::Vertices()
{
}

Vertices::Vertices(Graphics * graphics, int id)
	: graphics(graphics)
	, id(id)
{
	if (id == 1) {
		posX = 0; posY = 0; cR = 1.0f; cG = 1.0f; cB = 0.0f; size = 30.0f;
	}
	else {
		cR = std::uniform_real_distribution<float>{ 0.0f, 1.0f }(randomEngine);
		cG = std::uniform_real_distribution<float>{ 0.0f, 1.0f }(randomEngine);
		cB = std::uniform_real_distribution<float>{ 0.0f, 1.0f }(randomEngine);
		posX = std::uniform_real_distribution<float>{ -600.0f, 600.0f }(randomEngine);
		posY = std::uniform_real_distribution<float>{ -300.0f, 300.0f }(randomEngine);
		size = std::uniform_real_distribution<float>{ 30.0f, 50.0f }(randomEngine);
	}
}

Vertices::~Vertices()
{
	SAFE_RELEASE(rsState);
	SAFE_RELEASE(cBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARR(indices);
	SAFE_DELETE_ARR(vertices);
}

void Vertices::createVertBuffer()
{
	vertices = new vColor[4];

	vertices[0].pos = D3DXVECTOR3{ posX - size, posY - size,0.0f };
	vertices[1].pos = D3DXVECTOR3{ posX - size, posY + size,0.0f };
	vertices[2].pos = D3DXVECTOR3{ posX + size, posY - size,0.0f };
	vertices[3].pos = D3DXVECTOR3{ posX + size, posY + size,0.0f };

	vertices[0].color = D3DXCOLOR{ cR,cG,cB,1.0f };
	vertices[1].color = D3DXCOLOR{ cR,cG,cB,1.0f };
	vertices[2].color = D3DXCOLOR{ cR,cG,cB,1.0f };
	vertices[3].color = D3DXCOLOR{ cR,cG,cB,1.0f };

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D10_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(vColor) * 4;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D10_SUBRESOURCE_DATA));
		subData.pSysMem = vertices;

		HRESULT hr = this->graphics->GetDevice()->CreateBuffer
		(
			&desc,
			&subData,
			&vertexBuffer
		);
		assert(SUCCEEDED(hr));
	}

	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D10_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(uint) * 6;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D10_SUBRESOURCE_DATA));
		subData.pSysMem = indices;

		HRESULT hr = this->graphics->GetDevice()->CreateBuffer
		(
			&desc,
			&subData,
			&indexBuffer
		);
		assert(SUCCEEDED(hr));
	}
}

void Vertices::createConstBuffer()
{
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
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU : write, GPU : read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(TransformData);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = graphics->GetDevice()->CreateBuffer
		(
			&desc,
			nullptr,
			&cBuffer
		);
		assert(SUCCEEDED(hr));
	}

	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, 10, 10, 1);
	D3DXMatrixTranslation(&T, 0, 0, 0);
	D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(0)));

	//Create Rasterizer State
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.FrontCounterClockwise = false;

		//CullingÀü ClippingÈÄ

		HRESULT hr = graphics->GetDevice()->CreateRasterizerState(&desc, &rsState);

		assert(SUCCEEDED(hr));
	}
}

void Vertices::Update()
{
	this->left	 = this->posX - this->size;
	this->right  = this->posX + this->size;
	this->top	 = this->posY + this->size;
	this->bottom = this->posY - this->size;


		if (id == 1) {
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				world._42 += 5;
			}
			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				world._42 -= 5;
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				world._41 += 5;

			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				world._41 -= 5;

			}
		}
		else {
			if (timeGetTime() - timer > 500)
			{
				vertical = std::uniform_int_distribution<int>{ -1, 1 }(randomEngine);
				horizontal = std::uniform_int_distribution<int>{ -1, 1 }(randomEngine);
				timer = timeGetTime();
			}
			setDir();
			world._42 += vertical * 5;
			world._41 -= horizontal * 5;
			//this->square.left += vertical;
			//this->square.right += vertical;
			//this->square.top += horizontal;
			//this->square.bottom += horizontal;
		}

		D3DXMatrixTranspose(&data.WORLD, &world);
		D3DXMatrixTranspose(&data.VIEW, &view);
		D3DXMatrixTranspose(&data.PROJ, &proj);

		D3D11_MAPPED_SUBRESOURCE subResource;

		graphics->GetDeviceContext()->Map
		(
			cBuffer,
			0,
			D3D11_MAP_WRITE_DISCARD,
			0,
			&subResource
		);
		memcpy(subResource.pData, &data, sizeof(TransformData));

		graphics->GetDeviceContext()->Unmap(cBuffer, 0);

}

void Vertices::setDir()
{
	bool vCheck = false;
	vCheck |= this->left <= 300;
	vCheck |= this->right >= 600;

	bool hCheck = false;
	hCheck |= this->top <= 500;
	hCheck |= this->bottom >= 600;

	if (vCheck)
	{
		vertical *= -1;
	}
	if (hCheck)
	{
		horizontal *= -1;
	}
}

bool Vertices::isIntersect(Vertices*& vc)
{
	//Left   = pos-x
	//top    = pos+y
	//right  = pos+x
	//bottom = pos-y

	RECT temp;

	temp.left   = max(this->left , vc->left);
	temp.top    = max(this->top, vc->top);
	temp.right  = min(this->right, vc->right);
	temp.bottom = min(this->bottom, vc->bottom);

	bool bCheck = false;
	bCheck |= (temp.right - temp.left) < 0;
	bCheck |= (temp.bottom - temp.top) < 0;

	if (bCheck)
	{
		return false;
	}

	this->isActive = false;
	return true;

	return false;
}
