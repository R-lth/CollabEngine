#include "Input.h"

#include <cassert>

namespace Monkey
{
	Input* Input::instance = nullptr;

	Input::Input()
	{
		assert(!instance);
		instance = this;
	}

	Input::~Input()
	{
		instance = nullptr;
	}

	Input& Input::Get()
	{
		assert(instance);
		return *instance;
	}

	bool Input::Initialize(HWND hwnd)
	{
		mouseState.pos = { 0, 0 };
		mouseState.previousPos = mouseState.pos;

#if _USE_RAWINPUTDEVICE
		RAWINPUTDEVICE rid[2] = {};

		// Keyboard
		rid[0].usUsagePage = 0x01;
		rid[0].usUsage = 0x06;
		rid[0].dwFlags = 0;
		rid[0].hwndTarget = hwnd;

		// Mouse
		rid[1].usUsagePage = 0x01;
		rid[1].usUsage = 0x02;
		rid[1].dwFlags = 0;
		rid[1].hwndTarget = hwnd;

		if (!RegisterRawInputDevices(rid, 2, sizeof(RAWINPUTDEVICE)))
		{
			return false;
		}
#endif

		return true;
	}

	bool Input::OnHandleMessage(const MSG& msg)
	{
		switch (msg.message)
		{
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			HandleMsgKeyDown(msg.wParam, msg.lParam);
			return true;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			HandleMsgKeyUp(msg.wParam);
			return true;

		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			HandleMsgMouse(msg);
			return true;

		default:
			return false;
		}
	}

	bool Input::GetKey(UINT keyCode) const
	{
		assert(keyCode < keyCount);
		return keyStates[keyCode].isDown;
	}

	bool Input::GetKeyDown(UINT keyCode) const
	{
		assert(keyCode < keyCount);
		const InputState& state = keyStates[keyCode];
		return state.isDown && !state.wasDown;
	}

	bool Input::GetKeyUp(UINT keyCode) const
	{
		assert(keyCode < keyCount);
		const InputState& state = keyStates[keyCode];
		return !state.isDown && state.wasDown;
	}

	bool Input::GetMouseButton(MouseButton button) const
	{
		return GetMouseButtonState(button).isDown;
	}

	bool Input::GetMouseButtonDown(MouseButton button) const
	{
		const InputState& state = GetMouseButtonState(button);
		return state.isDown && !state.wasDown;
	}

	POINT Input::GetMousePosition() const
	{
		return mouseState.pos;
	}

	POINT Input::GetMouseDelta() const
	{
		POINT delta;
		delta.x = mouseState.pos.x - mouseState.previousPos.x;
		delta.y = mouseState.pos.y - mouseState.previousPos.y;
		return delta;
	}

	void Input::SavePreviousInputState()
	{
		for (InputState& state : keyStates)
		{
			state.wasDown = state.isDown;
		}

		mouseState.left.wasDown = mouseState.left.isDown;
		mouseState.right.wasDown = mouseState.right.isDown;
		mouseState.middle.wasDown = mouseState.middle.isDown;
		mouseState.previousPos = mouseState.pos;
	}

	void Input::HandleMsgKeyDown(WPARAM wParam, LPARAM lParam)
	{
		if (wParam >= keyCount)
		{
			return;
		}

		const bool wasAlreadyDown = (lParam & (1 << 30)) != 0;
		InputState& state = keyStates[static_cast<UINT>(wParam)];

		if (!wasAlreadyDown)
		{
			state.isDown = true;
		}
	}

	void Input::HandleMsgKeyUp(WPARAM wParam)
	{
		if (wParam >= keyCount)
		{
			return;
		}

		keyStates[static_cast<UINT>(wParam)].isDown = false;
	}

	void Input::HandleMsgMouse(const MSG& msg)
	{
		if (msg.message == WM_MOUSEMOVE ||
			msg.message == WM_LBUTTONDOWN || msg.message == WM_LBUTTONUP ||
			msg.message == WM_RBUTTONDOWN || msg.message == WM_RBUTTONUP ||
			msg.message == WM_MBUTTONDOWN || msg.message == WM_MBUTTONUP)
		{
			mouseState.pos = { GetXFromLParam(msg.lParam), GetYFromLParam(msg.lParam) };
		}

		switch (msg.message)
		{
		case WM_LBUTTONDOWN:
		{
			mouseState.left.isDown = true;
			SetCapture(msg.hwnd);
		}
			break;
		case WM_LBUTTONUP:
		{
			mouseState.left.isDown = false;
			ReleaseCapture();
		}
			break;
		case WM_RBUTTONDOWN:
		{
			mouseState.right.isDown = true;
			SetCapture(msg.hwnd);
		}
			break;
		case WM_RBUTTONUP:
		{
			mouseState.right.isDown = false;
			ReleaseCapture();
		}
			break;

		case WM_MBUTTONDOWN:
		{
			mouseState.middle.isDown = true;
			SetCapture(msg.hwnd);
		}
		break;

		case WM_MBUTTONUP:
		{
			mouseState.middle.isDown = false;
			ReleaseCapture();
		}
		break;
		}
	}

	Input::InputState& Input::GetMouseButtonState(MouseButton button)
	{
		switch (button)
		{
		case MouseButton::Left:
			return mouseState.left;
		case MouseButton::Right:
			return mouseState.right;
		default:
			assert(false);
			return mouseState.left;
		}
	}

	const Input::InputState& Input::GetMouseButtonState(MouseButton button) const
	{
		switch (button)
		{
		case MouseButton::Left:
			return mouseState.left;
		case MouseButton::Right:
			return mouseState.right;
		case MouseButton::Middle:
			return mouseState.middle;
		default:
			assert(false);
			return mouseState.left;
		}
	}

	int Input::GetXFromLParam(LPARAM lp)
	{
		return static_cast<int>(static_cast<short>(LOWORD(lp)));
	}

	int Input::GetYFromLParam(LPARAM lp)
	{
		return static_cast<int>(static_cast<short>(HIWORD(lp)));
	}
}