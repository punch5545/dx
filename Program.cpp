#include "stdafx.h"
#include "./Framework/Core/Window.h"
#include "./Framework/Core/Engine.h"

int APIENTRY WinMain
(
	HINSTANCE hInstance,
	HINSTANCE prevInstance,
	LPSTR lpszCmdParam,
	int nCmdShow
)
{
	Window::Create(hInstance, 1920, 1080);
	Window::Show();
	Window::Settings();
	Engine* engine = new Engine();
	
	while (Window::Update())
	{
		engine->Update();
		engine->Render();
	}
	SAFE_DELETE(engine);

	Window::Destroy();

	return 0;
}