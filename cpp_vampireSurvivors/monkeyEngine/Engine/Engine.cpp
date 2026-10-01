#include "Engine.h"
#include "../Level/Level.h"
#include "../Core/Input.h"
#include "../Render/Renderer.h"

#include <cassert>
#include <stdint.h> // int64_t(항상 8byte를 보장) 지원

namespace Monkey 
{
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		assert(!instance); // 중복 방지
		instance = this;
	}

	Engine::~Engine()
	{
		instance = nullptr;
	}

	Engine& Engine::Get()
	{
		assert(instance);
		return *instance;
	}

	void Engine::Run()
	{
		// 초기화
		StartUp();

		// 고해상도 타이머
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		int64_t currentTime, previousTime;
		currentTime = counter.QuadPart;
		previousTime = currentTime;

		// 게임 루프
		while (!isQuit) 
		{
			QueryPerformanceCounter(&counter);
			currentTime = counter.QuadPart;
			float deltaTime = static_cast<float>(currentTime - previousTime) / static_cast<float>(frequency.QuadPart);
			//if (deltaTime < targetFrameTime) { continue; }
			previousTime = currentTime;
			
			ProcessInput();
			OnInitialized();
			BeginPlay();
			Update(deltaTime);
			Render();

			if (nextLevel) // 레벨 전환 
			{
				if (mainLevel) { mainLevel.reset(); }
				mainLevel = std::move(nextLevel); // 소유권 이전
				nextLevel.reset();
			}
			if (mainLevel) { mainLevel->ProcessAddAndDestroyActors(); }

			SavePreviousInputState(); // 입력 기록
		}

		//
		ShutDown();
	}

	void Engine::Quit()
	{
		isQuit = true;
	}

	void Engine::StartUp()
	{
		const wchar_t* className = L"D2DGame";
		const wchar_t* windowName = L"세기말 서바이벌. 게임 종료는 Esc 키";
		if (__super::Create(className, windowName, setting.width, setting.height) == false) 
		{
			Quit();
			return; 
		}

		input = std::make_unique<Input>();
		renderer = std::make_unique<Renderer>(m_hWnd, setting.width, setting.height);

		input->Initialize(m_hWnd);
	}

	void Engine::ShutDown()
	{
		__super::Destroy();
	}

	void Engine::OnInitialized()
	{
		if (!mainLevel || mainLevel->HasInitialized()) { return; }
		mainLevel->OnInitialized();
	}

	void Engine::BeginPlay()
	{
		if (!mainLevel) { return; }
		mainLevel->BeginPlay();
	}

	void Engine::ProcessInput()
	{
		// 메세지 큐에서 가져온 이벤트
		MSG msg = {};

		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) { Quit(); break; }

			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// DispatchMessage가 OS 내부적으로 메시지를 처리하고 나서, WndProc에서 처리해야 함
			input->OnHandleMessage(msg);
		}
	}

	void Engine::SavePreviousInputState()
	{
		input->SavePreviousInputState();
	}

	void Engine::Update(float deltaTime)
	{
		//std::cout << "deltaTime: " << deltaTime << ", FPS: " << 1 / deltaTime << '\n';

		if (!mainLevel) { return; }
		mainLevel->Update(deltaTime);
	}

	void Engine::Render()
	{
		if (!mainLevel) { return; }
		mainLevel->Render();
		renderer->Render();
	}
}