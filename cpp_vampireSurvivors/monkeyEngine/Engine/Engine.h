#pragma once

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2026-06-04
// Copyright    : 게임인재원 8기
// Description  : Engine 클래스
//
//              - 
//---------------------------------------------------------------------------------------------------------

#include "../Core/Core.h"
#include "WndEngine.h"

#include <unordered_map>

namespace Monkey
{
	class Level;
	class Input;
	class Renderer;

	class Monkey_API Engine : private WndEngine
	{
	public:
		enum class State : int
		{
			Game3D = 0,
			Menu = 1
		};

	public:
		Engine();
		~Engine();

		Engine(const Engine&) = delete;
		Engine operator = (const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine operator = (Engine&&) = delete;

		// 싱글톤
		static Engine& Get();

	public:
		void Run();
		void Quit();

		template<typename T, typename = std::enable_if_t<std::is_base_of<Level, T>::value>> // Level이나 Level 파생만 가능
		inline void AddNewLevel() { nextLevel = std::make_shared<T>(); }

		// inline getter
		HWND GetHandle() const { return m_hWnd; }
		int GetWidth() const { return setting.width; }
		int GetHeight() const { return setting.height; }
		State GetState() const { return state; }

	private:
		void StartUp() override;
		void ShutDown() override;

		// 초기화
		void OnInitialized(); // 레벨
		void BeginPlay(); // 액터

		// 입력
		void ProcessInput();
		void SavePreviousInputState();

		// 로직 갱신
		// void Tick(float delaTime);
		void Update(float deltaTime);

		// 화면 출력
		void Render();

	protected:
		std::shared_ptr<Level> mainLevel;
		std::shared_ptr<Level> nextLevel;

		std::unique_ptr<Input> input;
		std::unique_ptr<Renderer> renderer;

		State state = State::Game3D;

	private:
		static Engine* instance;

		bool isQuit = false;
		float accumulator = 0.f;
	};
}



