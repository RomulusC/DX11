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
	for (auto i = 0; i < 1000; i++)
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
	// Getting camera world space co-ordinates using SIMD Vector Math
	DirectX::XMMATRIX cameraMat = m_window.Gfx().m_fpsCamera.GetCameraMatXM(); // Get camera matrix
	DirectX::XMVECTOR transVec = cameraMat.r[3];							   // Get camera matrix Translation vector
	cameraMat = DirectX::XMMatrixTranspose(cameraMat);						   // Transpose to have access to rows (DX SIMD Matrix object only can access columns)
	transVec = DirectX::XMVector3Transform(transVec,cameraMat);				   // Transform Camera Translation vector to the original Camera Matrix row components
	transVec = DirectX::XMVectorNegate(transVec);							   // Negate, remember camera transformations are opposite of inputs. Move camera forward means push world space away from camera
	
	DirectX::XMFLOAT3 cameraPos;											   
	DirectX::XMStoreFloat3(&cameraPos, transVec);							   // Bring data from SIMD to regular object

	// Print camera world position to application name
	std::ostringstream ss;
	ss << std::fixed << std::setprecision(2) << "World_Pos: " << "X: " << cameraPos.x << " Y:" << cameraPos.y << " Z:" << cameraPos.z;
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