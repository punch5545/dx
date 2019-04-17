#pragma once
#pragma comment(lib, "winmm.lib")
#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

//Window
#include <Windows.h>
#include <assert.h>

//STL
#include <typeinfo>
#include <vector>
#include <iostream>
#include <random>

//DirectX
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

typedef unsigned int uint;
typedef unsigned long ulong;

#define SAFE_RELEASE(p)		{ if(p) {(p)->Release(); (p) = nullptr; } }
#define SAFE_DELETE(p)		{ if(p) { delete    (p); (p) = nullptr; } }
#define SAFE_DELETE_ARR(p)  { if(p) { delete[]  (p); (p) = nullptr; } }

//main system
#include "./Framework/Core/Settings.h"
#include "./Framework/Core/Context.h"

//Sub system
#include "./Framework/Core/Subsystem/Graphics.h"