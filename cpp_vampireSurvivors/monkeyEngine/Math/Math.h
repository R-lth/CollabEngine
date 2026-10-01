#pragma once

#include "../Core/Core.h"

#include <DirectXMath.h>

namespace Monkey
{
	class Vector2 
	{
	public:
		Vector2() = default;
		Vector2(int x, int y);
		~Vector2() = default;

	public:
		static float Dot(const Vector2& a, const Vector2& b);

	public:
		// 연산자 오버로딩
		// 사칙연산
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(const Vector2& other) const;
		Vector2 operator/(const Vector2& other) const;

		Vector2 operator*(int value) const;

		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

	public:
		int x; // 좌우 
		int y; // 상하

	public:
		static const Vector2 Zero;
		static const Vector2 One;
		static const Vector2 Right;
		static const Vector2 Up;
	};

	class Monkey_API Vector3
	{
	public:
		Vector3() = default;
		Vector3(float x, float y, float z);
		~Vector3() = default;

	public:
		// 연산자 오버로딩
		// 사칙연산
		Vector3 operator+(const Vector3& other) const;
		Vector3 operator-(const Vector3& other) const;
		Vector3 operator*(const Vector3& other) const;
		Vector3 operator/(const Vector3& other) const;

		Vector3 operator*(int value) const;

		bool operator==(const Vector3& other) const;
		bool operator!=(const Vector3& other) const;

	public:
		Vector3& operator+=(const Vector3& other);
		Vector3& operator-=(const Vector3& other);

		Vector3 operator*(float value) const;

		float Length() const;
		Vector3 Normalize() const;

		DirectX::XMVECTOR ToXMVECTOR() const;

	public:
		float x; // 좌우
		float y; // 상하(높이)
		float z; // 깊이(전방)

	public:
		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 Right;
		static const Vector3 Up;
		static const Vector3 Forward;
	};

	/*class Monkey_API Vector4
	{
	public:
		Vector4(float x = 0, float y = 0, float z = 0, float w = 1);
		~Vector4() = default;

	public:


	public:
		float x;
		float y;
		float z;
		float w;

	public:

	};*/
}



