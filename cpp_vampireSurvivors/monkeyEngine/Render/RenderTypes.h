#pragma once

#include "../Core/Core.h"

#include <vector>
#include <cstdint>

#include <d2d1.h> 
#include <DirectXMath.h>

namespace Monkey
{
    class Monkey_API Color
    {
    public:
        Color() = default;
        Color(float red, float green, float blue);

    public:
        Color& operator=(const D2D1_COLOR_F& other);

    public:
        static const Color White;
        static const Color Black;
        static const Color Red;
        static const Color Green;
        static const Color Blue;

    public:
        float red = 1.0f;
        float green = 1.0f;
        float blue = 1.0f;
        static constexpr float alpha = 1.0f;
    };

    enum class Monkey_API Primitive
    {
        None,
        LineList,
        TriangleList
    };

    struct Monkey_API Vertex
    {
        DirectX::XMFLOAT3 position = { 0, 0, 0 };
        Color color = Color::White;
    };

    struct Monkey_API RenderCommand
    {
        Primitive primitiveType = Primitive::TriangleList;
        std::vector<Vertex> vertices;
        std::vector<int> indices;
        DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
    };
};