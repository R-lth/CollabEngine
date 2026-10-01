#pragma once

#include "../Core/Core.h"

#include <Windows.h>

namespace Monkey
{
	class Monkey_API Input
	{
		friend class Engine;

	public:
		enum class MouseButton
		{
			Left,
			Right,
			Middle
		};

	private:
		struct InputState
		{
			bool isDown = false;
			bool wasDown = false;
		};

		struct MouseState
		{
			POINT pos{ 0, 0 };
			POINT previousPos{ 0, 0 };
			InputState left = {};
			InputState right = {};
			InputState middle = {};
		};

	public:
		Input();
		~Input();

		static Input& Get();

		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;
		Input(Input&&) = delete;
		Input& operator=(Input&&) = delete;

	public:
		bool Initialize(HWND hwnd);
		bool OnHandleMessage(const MSG& msg);

		// 키보드 (게임 조작)
		bool GetKey(UINT keyCode) const;     // 이동
		bool GetKeyDown(UINT keyCode) const; // 점프, 공격, esc
		bool GetKeyUp(UINT keyCode) const;   // 

		// 마우스 (카메라 회전, UI)
		bool GetMouseButton(MouseButton button) const; 
		bool GetMouseButtonDown(MouseButton button) const; // 클릭
		POINT GetMousePosition() const; // Hover = 마우스가 UI 위치에 있는지
		POINT GetMouseDelta() const;

		void SavePreviousInputState();

	private:
		void HandleMsgKeyUp(WPARAM wParam);
		void HandleMsgKeyDown(WPARAM wParam, LPARAM lParam);
		void HandleMsgMouse(const MSG& msg);

		InputState& GetMouseButtonState(MouseButton button);
		const InputState& GetMouseButtonState(MouseButton button) const;

		static int GetXFromLParam(LPARAM lp);
		static int GetYFromLParam(LPARAM lp);

	private:
		static constexpr UINT keyCount = 256;
		InputState keyStates[keyCount] = {};
		MouseState mouseState = {};

		static Input* instance;
	};
}



