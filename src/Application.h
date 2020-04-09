#pragma once
#include "Window.h"
#include "Timer.h"
#include "FpsCamera.h"

class Application
{
public:
	Application(unsigned int _xRes, unsigned _yRes, const char* _name);
	// master frame / message loop
	int Go();
	~Application();
private:
	void DoFrame();
private:
	Window m_window;
	Timer m_timer;
	std::vector<std::unique_ptr<class Box>> m_boxes;
};