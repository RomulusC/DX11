#pragma once
#include <queue>
#include <bitset>
#include <optional>

class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press = 0,
			Release,
			Invalid,
		};
	private:
		Type m_type;
		unsigned char m_code;
	public:
		Event() noexcept
			: m_type(Type::Invalid)
			, m_code(0u)
		{}
		
		Event(Type _type, unsigned int _code) noexcept
			: m_type(_type)
			, m_code(_code)
		{}
		
		bool IsPressed() const noexcept
		{
			return m_type == Type::Press;
		}
		bool IsReleased() const noexcept
		{
			return m_type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return m_type == Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return m_code;
		}
	};
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	// key events
	bool KeyIsPressed(unsigned int _keycode) const noexcept;
	std::optional<Keyboard::Event> ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;
	// char events
	std::optional<char> ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	// auto repeat control
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool IsAutoRepeatEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char _keycode) noexcept;
	void OnKeyRelease(unsigned char _keycode) noexcept;
	void OnChar(char _char) noexcept;
	void ClearState() noexcept;

	template<typename T>
	static void TrimBuffer(std::queue<T>& _buffer) noexcept;

	static constexpr unsigned int s_nKeys = 256u; // maximum count of virtual keys
	static constexpr unsigned int s_bufferSize = 16u;
	bool m_autoRepeatEnabled = false;
	std::bitset<s_nKeys> m_keyStates;
	std::queue<Keyboard::Event> m_keyBuffer;
	std::queue<char> m_charBuffer;	
};