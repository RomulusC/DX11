#pragma once
#include "Window.h"
#include "Timer.h"
#include <DirectXMath.h>

class Application
{

public:
	Application(int _xRes, int _yRes, const char* _name);
	int Go();
private:
	void DoFrame();
private: 
	Window m_window;
	std::string m_name;
	Timer m_timer;
	DirectX::XMMATRIX m_cameraTransform;
};