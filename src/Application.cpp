#include "Application.h"
#include "Box.h"
#include <memory>

#include <sstream>
#include <iomanip>

Application::Application(unsigned int _xRes, unsigned _yRes, const char* _name)
	: m_window(_xRes, _yRes, _name)
	, m_timer(Timer())
	, m_boxes(std::vector<std::unique_ptr<class Box>>())
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 80; i++)
	{
		m_boxes.push_back(std::make_unique<Box>(
			m_window.Gfx(), rng, adist,
			ddist, odist, rdist
			));
	}
	//m_window.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	m_window.m_keyboard.EnableAutoRepeat();
}

int Application::Go()
{
	while (true)
	{
		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessage())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		DoFrame();
	}
}

Application::~Application()
{}

void Application::DoFrame()
{
	
	DirectX::XMFLOAT4X4 temp;
	DirectX::XMStoreFloat4x4(&temp, m_window.Gfx().m_fpsCamera.GetCameraMatXM());
	DirectX::XMFLOAT4 worldPos = {};
	worldPos.x += temp._11 * temp._41;
	worldPos.y += temp._21 * temp._41;
	worldPos.z += temp._31 * temp._41;

	worldPos.x += temp._12 * temp._42;
	worldPos.y += temp._22 * temp._42;
	worldPos.z += temp._32 * temp._42;

	worldPos.x += temp._13 * temp._43;
	worldPos.y += temp._23 * temp._43;
	worldPos.z += temp._33 * temp._43;


	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << "World_Pos: " << "X: " << worldPos.x << " Y:" << worldPos.y << " Z:" << worldPos.z;
	m_window.ChangeTitle(ss.str().c_str());
	
	auto dt = m_timer.Mark();
	m_window.Gfx().m_fpsCamera.Update(dt, m_window.m_keyboard, m_window.m_mouse);
	m_window.Gfx().ClearBuffer(.28f, .6f, 1.0f);
	for (auto& b : m_boxes)
	{
		b->Update(dt);
		b->Draw(m_window.Gfx());
	}
	m_window.Gfx().EndFrame();
}