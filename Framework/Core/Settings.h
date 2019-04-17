#pragma once
class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;
		return instance;
	}

	auto getWindowInstance() const->HINSTANCE { return hInstance; }
	void setWindowInstance(HINSTANCE hInstance) { this->hInstance = hInstance; }

	auto getWindowHandle() const->HWND { return handle; }
	void setWindowHandle(HWND handle) { this->handle = handle; }

	auto getWidth() const -> const float { return windowSize.x; }
	void setWidth(const float& x) { this->windowSize.x = x; }
	
	auto getHeight() const -> const float { return windowSize.y; }
	void setHeight(const float& y) { this->windowSize.y = y; }
private:
	Settings();
	~Settings() = default;

	HINSTANCE hInstance;
	HWND handle;
	D3DXVECTOR2 windowSize; // float x, y;

};