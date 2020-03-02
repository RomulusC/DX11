#include "Keyboard.h"

bool Keyboard::KeyIsPressed(unsigned int _keycode) const noexcept
{
	return m_keyStates[_keycode] == 1;
}

std::optional<Keyboard::Event> Keyboard::ReadKey() noexcept
{
	std::optional<Keyboard::Event> e;
	if (!m_keyBuffer.empty())
	{
		e = m_keyBuffer.front();
		m_keyBuffer.pop();
	}
	return e;	
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return m_keyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	while (m_keyBuffer.empty() != true)
	{
		m_keyBuffer.pop();
	}
}

std::optional<char> Keyboard::ReadChar() noexcept
{
	std::optional<char> c;
	if (!m_keyBuffer.empty())
	{
		c = m_charBuffer.front();
		m_charBuffer.pop();
	}
	return c;
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return m_charBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	while (m_charBuffer.empty() != true)
	{
		m_charBuffer.pop();
	}
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutoRepeat() noexcept
{
	m_autoRepeatEnabled = true;
}

void Keyboard::DisableAutoRepeat() noexcept
{
	m_autoRepeatEnabled = false;
}

bool Keyboard::IsAutoRepeatEnabled() const noexcept
{
	return m_autoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char _keycode) noexcept
{
	m_keyStates[_keycode] = true;
	m_keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, _keycode));
	TrimBuffer(m_keyBuffer);
}

void Keyboard::OnKeyRelease(unsigned char _keycode) noexcept
{
	m_keyStates[_keycode] = false;
	m_keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, _keycode));
	TrimBuffer(m_keyBuffer);
}

void Keyboard::OnChar(char _char) noexcept
{
	m_charBuffer.push(_char);
	TrimBuffer(m_charBuffer);
}

void Keyboard::ClearState() noexcept
{
	m_keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& _buffer) noexcept
{
	while (_buffer.size() > s_bufferSize)
	{
		_buffer.pop();
	}
}