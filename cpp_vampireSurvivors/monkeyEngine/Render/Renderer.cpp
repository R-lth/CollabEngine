#include "Renderer.h"
#include "../Render/Graphics/Graphics.h"
#include "../Engine/Engine.h"

#include <algorithm>
#include <cassert>

namespace Monkey
{
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(HWND hwnd, int width, int height)
	{
		assert(instance == nullptr);
		instance = this;

		assert(hwnd != nullptr);

		graphics = std::make_unique<Graphics>();
		const bool initialized = graphics->Initialize();
		assert(initialized);
	}

	Renderer::~Renderer()
	{
		instance = nullptr;
	}

	Renderer& Renderer::Get()
	{
		assert(instance != nullptr);
		return *instance;
	}

	void Renderer::Submit(Primitive& primitiveType, const std::vector<Vertex>& vertices, const std::vector<int>& indices, const DirectX::XMMATRIX& world)
	{
		RenderCommand command;
		command.primitiveType = primitiveType;
		command.vertices = vertices;
		command.indices = indices;
		command.world = world;

		// renderCommands에 Actor들의 RenderCommand를 저장
		renderCommands.push_back(command); 
	}

	void Renderer::Render()
	{
		graphics->BeginFrame();
		
		switch (Engine::Get().GetState())
		{
		case Engine::State::Game3D:
		{
			graphics->Clear(Color::Blue);
			graphics->ClearFrameBuffer();
			graphics->UpdateCamera();

			// 렌더링 파이프라인
			for (RenderCommand& command : renderCommands)
			{
				graphics->Draw(command);
			}

			// 화면에 출력
			graphics->PresentFrameBuffer();
		}
			break;
		case Engine::State::Menu:
		{
			graphics->Clear(Color::White);
			graphics->PresentUI(uiText);
		}
			break;
		default:
			break;
		}
		graphics->EndFrame();
		renderCommands.clear();
	}

	Graphics& Renderer::GetGraphics()
	{ 
		return *graphics; 
	};
}