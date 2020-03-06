#pragma once
#include "Window.h"
#include "Mouse.h"


std::pair<int, int> Mouse::GetPos() const noexcept
{
	return std::pair<int, int>(m_xPos, m_yPos);
}

int Mouse::GetPosX() const noexcept
{
	return m_xPos;
}

int Mouse::GetPosY() const noexcept
{
	return m_yPos;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return m_leftIsPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return m_rightIsPressed;
}

bool Mouse::IsMMClickReleased() const noexcept
{
	return m_mmClickPressed;
}

bool Mouse::IsMouseInWindow() const noexcept
{
	return m_isMouseInWindow;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
	std::optional<Event> e;
	if (!m_eventBuffer.empty())
	{
		e = m_eventBuffer.front();
		m_eventBuffer.pop();
	}
	return e;
}

void Mouse::Flush() noexcept
{
	while (!m_eventBuffer.empty())
	{
		m_eventBuffer.pop();
	}
}

void Mouse::OnMouseMove(int _x, int _y) noexcept
{
	m_xPos = _x;
	m_yPos = _y;
	m_eventBuffer.push(Mouse::Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int _x, int _y) noexcept
{
	
	m_leftIsPressed = true;
	m_eventBuffer.push(Mouse::Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int _x, int _y) noexcept
{	
	
	m_leftIsPressed = false;
	m_eventBuffer.push(Mouse::Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int _x, int _y) noexcept
{
	
	m_rightIsPressed = true;
	m_eventBuffer.push(Mouse::Event(Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int _x, int _y) noexcept
{
	
	m_rightIsPressed = false;
	m_eventBuffer.push(Mouse::Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMMPressed(int _x, int _y) noexcept
{
	
	m_mmClickPressed = true;
	m_eventBuffer.push(Mouse::Event(Event::Type::MMClickPress, *this));
	TrimBuffer();
}

void Mouse::OnMMReleased(int _x, int _y) noexcept
{
	
	m_mmClickPressed = false;
	m_eventBuffer.push(Mouse::Event(Event::Type::MMClickRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUpdate(int _x, int _y, int _delta) noexcept
{
	m_wheelDelta += _delta;
	// generate wheel updates every WHEEL_DELTA (= 120)
	while (m_wheelDelta >= WHEEL_DELTA)
	{
		m_wheelDelta -= WHEEL_DELTA;
		OnWheelUp(_x, _y);
	}
	while (m_wheelDelta <= -WHEEL_DELTA)
	{
		m_wheelDelta += WHEEL_DELTA;
		OnWheelDown(_x, _y);
	}
}

void Mouse::OnWheelUp(int _x, int _y) noexcept
{
	
	m_eventBuffer.push(Mouse::Event(Event::Type::ScrollUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int _x, int _y) noexcept
{
	
	m_eventBuffer.push(Mouse::Event(Event::Type::ScrollDown, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeaveWindow() noexcept
{
	m_isMouseInWindow = false;
	m_eventBuffer.push(Mouse::Event(Event::Type::LeaveWindow, *this));

}

void Mouse::OnMouseEnterWindow() noexcept
{
	m_isMouseInWindow = true;
	m_eventBuffer.push(Mouse::Event(Event::Type::EnterWindow, *this));
}

void Mouse::TrimBuffer() noexcept
{
	while (m_eventBuffer.size() > m_bufferSize)
	{
		m_eventBuffer.pop();
	}
}

