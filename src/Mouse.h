#pragma once
#include <queue>
#include <bitset>
#include <optional>

class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress = 0,
			LRelease,
			RPress,
			RRelease,
			MMClickPress,
			MMClickRelease,
			ScrollUp,
			ScrollDown,
			Move,
			EnterWindow,
			LeaveWindow,
			Invalid,
		};
	private:
		Type m_type;
		bool m_leftIsPressed;
		bool m_rightIsPressed;
		bool m_mmClickPressed;
		int m_xPos;
		int m_yPos;
	public:
		Event() noexcept
			: m_type(Type::Invalid)
			, m_leftIsPressed(false)
			, m_rightIsPressed(false)
			, m_mmClickPressed(false)
			, m_xPos(0)
			, m_yPos(0)
		{}

		Event(Type _type, const Mouse& _parent) noexcept
			: m_type(_type)
			, m_leftIsPressed(_parent.m_leftIsPressed)
			, m_rightIsPressed(_parent.m_rightIsPressed)
			, m_mmClickPressed(_parent.m_mmClickPressed)
			, m_xPos(_parent.m_xPos)
			, m_yPos(_parent.m_yPos)
		{}
		bool IsValid() noexcept
		{
			return m_type != Type::Invalid;
		}
		Type GetType() noexcept
		{
			return m_type;
		}
		std::pair<int, int> GetPos() noexcept
		{
			return std::pair<int, int>(m_xPos, m_yPos);
		}
		int GetPosX() noexcept
		{
			return m_xPos;
		}
		int GetPosY() noexcept
		{
			return m_yPos;
		}
		bool IsLeftPressed()
		{
			return m_leftIsPressed;
		}
		bool IsRightPressed()
		{
			return m_rightIsPressed;
		}
		bool IsMMClickPressed()
		{
			return m_mmClickPressed;
		}
	};

	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	// Mouse events
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsMMClickReleased() const noexcept;
	bool IsMouseInWindow() const noexcept;
	std::optional<Event> Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return m_eventBuffer.empty();
	}
	void Flush() noexcept;
private:
	void OnMouseMove(int _x, int _y) noexcept;
	void OnLeftPressed(int _x, int _y) noexcept;
	void OnLeftReleased(int _x, int _y) noexcept;
	void OnRightPressed(int _x, int _y) noexcept;
	void OnRightReleased(int _x, int _y) noexcept;
	void OnMMPressed(int _x, int _y) noexcept;
	void OnMMReleased(int _x, int _y) noexcept;
	void OnWheelUpdate(int _x, int _y, int _delta) noexcept;
	void OnWheelUp(int _x, int _y) noexcept;
	void OnWheelDown(int _x, int _y) noexcept;
	void OnMouseLeaveWindow() noexcept;
	void OnMouseEnterWindow() noexcept;
	void TrimBuffer() noexcept;
private:
	static constexpr unsigned int m_bufferSize = 16u;
	int m_xPos;
	int m_yPos;
	bool m_leftIsPressed = false;
	bool m_rightIsPressed = false;
	bool m_mmClickPressed = false;
	int m_wheelDelta;
	bool m_isMouseInWindow = false;
	std::queue<Event> m_eventBuffer;
};