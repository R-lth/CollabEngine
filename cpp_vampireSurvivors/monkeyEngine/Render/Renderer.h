#pragma once

#include "../Core/Core.h"
#include "../Render/RenderTypes.h"
#include "../Render/Graphics/Graphics.h"

#include <vector>
#include <memory>

namespace Monkey
{
	class Graphics;

	class Monkey_API Renderer
	{
	public:
		Renderer(HWND hwnd, int width, int height);
		~Renderer();

		static Renderer& Get();

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	public:
		void Submit(Primitive& primitiveType, const std::vector<Vertex>& vertices, const std::vector<int>& indices, const DirectX::XMMATRIX& world);
		void Render();

		Graphics& GetGraphics();

	public:
		std::wstring uiText[3];

	private:
		static Renderer* instance;

		std::unique_ptr<Graphics> graphics;
		std::vector<RenderCommand> renderCommands;
	};
}

