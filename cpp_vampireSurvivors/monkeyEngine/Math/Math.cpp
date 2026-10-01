#include "Math.h"

#include <cmath>
#include <cassert>

namespace Monkey
{
	// 윈도우 좌표계 기준
	const Vector2 Vector2::Zero(0, 0);
	const Vector2 Vector2::One(1, 1);
	const Vector2 Vector2::Right(1, 0);
	const Vector2 Vector2::Up(0, -1);

	float Vector2::Dot(const Vector2& a, const Vector2& b)
	{
		return static_cast<float>((a.x * b.x) + (a.y * b.y));
	}

	Vector2::Vector2(int x, int y) : x(x), y(y)
	{
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator*(const Vector2& other) const
	{
		return Vector2(x * other.x, y + other.y);
	}

	Vector2 Vector2::operator/(const Vector2& other) const
	{
		assert(other.x != 0);
		assert(other.y != 0);

		return Vector2(x / other.x, y / other.y);
	}
	
	Vector2 Vector2::operator*(int value) const
	{
		return Vector2(x * value, y * value);
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		return (x == other.x && y == other.y);
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return (x != other.x) && (y != other.y);
	}

	// 윈도우 좌표계 기준
	const Vector3 Vector3::Zero(0, 0, 0);
	const Vector3 Vector3::One(1, 1, 1);
	const Vector3 Vector3::Right(1, 0, 0);
	const Vector3 Vector3::Up(0, -1, 0);
	const Vector3 Vector3::Forward(0, 0, 1);

	Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z)
	{
	}

	Vector3 Vector3::operator+(const Vector3& other) const
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}
	Vector3 Vector3::operator-(const Vector3& other) const
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}
	Vector3 Vector3::operator*(const Vector3& other) const
	{
		return Vector3(x * other.x, y * other.y, z * other.z);
	}
	Vector3 Vector3::operator/(const Vector3& other) const
	{
		assert(other.x != 0.f);
		assert(other.y != 0.f);
		assert(other.z != 0.f);

		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	Vector3 Vector3::operator*(int value) const
	{
		return Vector3(x * value, y * value, z * value);
	}

	bool Vector3::operator==(const Vector3& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	bool Vector3::operator!=(const Vector3& other) const
	{
		return (x != other.x) && (y != other.y) && (z != other.z);
	}

	Vector3 Vector3::operator*(float value) const
	{
		return Vector3(x * value, y * value, z * value);
	}

	Vector3& Vector3::operator+=(const Vector3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	float Vector3::Length() const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3 Vector3::Normalize() const
	{
		float length = Length();

		if (length <= 0.0f)
		{
			return Vector3::Zero;
		}

		return Vector3(x / length, y / length, z / length);
	}

	DirectX::XMVECTOR Vector3::ToXMVECTOR() const
	{
		return DirectX::XMVectorSet(x, y, z, 0.0f);
	}
}