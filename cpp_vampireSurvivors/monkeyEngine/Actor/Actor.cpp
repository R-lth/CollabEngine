#include "Actor.h"
#include "../Engine/Engine.h"
#include "../Core/Input.h"
#include "../Render/Renderer.h"

namespace Monkey
{
	void Actor::SetPosition(const Vector3& newPosition)
	{
		if (transform.position == newPosition) { return; }
		transform.position = newPosition;
	}

	void Actor::BeginPlay() 
	{
		hasBeganPlay = true;
	}

	void Actor::Update(float deltaTime) 
	{
	}

	void Actor::Render() 
	{
		if (!IsActive()) { return; }

		// 공간 변환
		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(
			DirectX::XMConvertToRadians(transform.rotation.x),
			DirectX::XMConvertToRadians(transform.rotation.y),
			DirectX::XMConvertToRadians(transform.rotation.z)
		);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(transform.position.x, transform.position.y, transform.position.z);
		world = scale * rotation * translation;
		
		// 그리기
		Monkey::Renderer::Get().Submit(primitiveType, vertices, indices, world);
	}

	void Actor::OnDestroyed()
	{
	}

	void Actor::OnCollision(const std::shared_ptr<Actor>& other)
	{
	}

	void Actor::QuitGame()
	{
		Engine::Get().Quit();
	}

	void Actor::Destroy()
	{
		hasExpired = true;
	}

	Actor::Layer Actor::GetLayer()
	{
		return layer;
	}

	Actor::AABB Actor::GetAABB() const
	{
		AABB box;

		Vector3 center = transform.position + collisionOffset;

		Vector3 half = Vector3(
			collisionSize.x * transform.scale.x * 0.5f,
			collisionSize.y * transform.scale.y * 0.5f,
			collisionSize.z * transform.scale.z * 0.5f
		);

		box.min = center - half;
		box.max = center + half;

		return box;
	}

	Actor::SphereCollider Actor::GetSphereCollider() const
	{
		SphereCollider sphere;

		sphere.center = transform.position + collisionOffset;

		float maxScale = std::max(
			transform.scale.x,
			std::max(transform.scale.y, transform.scale.z)
		);

		sphere.radius = collisionRadius * maxScale;

		return sphere;
	}

	void Actor::SetGrid(const Color& xAxis, const Color& yAxis, const Color& zAxis, const Color& grid)
	{
		primitiveType = Monkey::Primitive::LineList;
		vertices.clear();
		indices.clear();

		auto AddLine = [&](const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b, const Color& color)
			{
				const uint32_t base = static_cast<uint32_t>(vertices.size());

				vertices.push_back({ a, color });
				vertices.push_back({ b, color });

				indices.push_back(base);
				indices.push_back(base + 1);
			};

		const float length = 3.0f;

		AddLine({ 0.0f, 0.0f, 0.0f }, { length, 0.0f, 0.0f }, xAxis);
		AddLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, length, 0.0f }, yAxis);
		AddLine({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, length }, zAxis);

		const float desiredHalfSize = 8.0f;
		const float cellSize = 1.5f;

		const int slices = static_cast<int>(std::round(desiredHalfSize / cellSize));
		const float halfSize = slices * cellSize;

		for (int i = -slices; i <= slices; ++i)
		{
			const float pos = i * cellSize;

			AddLine({ -halfSize, 0.0f, pos }, { halfSize, 0.0f, pos }, grid);
			AddLine({ pos, 0.0f, -halfSize }, { pos, 0.0f, halfSize }, grid);
		}

		SetColliderType(ColliderType::None);
		SetCollisionEnabled(false);
	}

	void Actor::SetMeshCone(const Color& color)
	{
		primitiveType = Monkey::Primitive::TriangleList;
		vertices.clear();
		indices.clear();

		const int segmentCount = 24;
		const float radius = 0.5f;
		const float height = 1.5f;

		const uint32_t apexIndex = static_cast<uint32_t>(vertices.size());
		vertices.push_back({ { 0.0f, height, 0.0f }, color });

		const uint32_t centerIndex = static_cast<uint32_t>(vertices.size());
		vertices.push_back({ { 0.0f, 0.0f, 0.0f }, color });

		const uint32_t ringStartIndex = static_cast<uint32_t>(vertices.size());

		for (int i = 0; i < segmentCount; ++i)
		{
			const float angle = DirectX::XM_2PI * i / segmentCount;

			DirectX::XMFLOAT3 p(
				cosf(angle) * radius,
				0.0f,
				sinf(angle) * radius
			);

			vertices.push_back({ p, color });
		}

		for (int i = 0; i < segmentCount; ++i)
		{
			const uint32_t p0 = ringStartIndex + i;
			const uint32_t p1 = ringStartIndex + ((i + 1) % segmentCount);

			// 옆면
			indices.push_back(apexIndex);
			indices.push_back(p1);
			indices.push_back(p0);

			// 바닥면
			indices.push_back(centerIndex);
			indices.push_back(p0);
			indices.push_back(p1);
		}

		SetColliderType(ColliderType::SphereCollider);
		SetCollisionOffset(Vector3(0.f, 0.75f * transform.scale.y, 0.f));
		SetCollisionRadius(0.45f);
	}

	void Actor::SetMeshSphere(const Color& color)
	{
		primitiveType = Monkey::Primitive::TriangleList;
		vertices.clear();
		indices.clear();

		const int latitudeCount = 16;
		const int longitudeCount = 32;
		const float radius = 0.5f;

		auto MakeSpherePoint = [&](float theta, float phi) -> DirectX::XMFLOAT3
			{
				return DirectX::XMFLOAT3(
					radius * sinf(theta) * cosf(phi),
					radius * cosf(theta),
					radius * sinf(theta) * sinf(phi)
				);
			};

		// 정점 생성: latitudeCount + 1, longitudeCount + 1
		for (int lat = 0; lat <= latitudeCount; ++lat)
		{
			const float theta = DirectX::XM_PI * lat / latitudeCount;

			for (int lon = 0; lon <= longitudeCount; ++lon)
			{
				const float phi = DirectX::XM_2PI * lon / longitudeCount;
				vertices.push_back({ MakeSpherePoint(theta, phi), color });
			}
		}

		const int rowVertexCount = longitudeCount + 1;

		auto GetIndex = [&](int lat, int lon) -> uint32_t
			{
				return static_cast<uint32_t>(lat * rowVertexCount + lon);
			};

		for (int lat = 0; lat < latitudeCount; ++lat)
		{
			for (int lon = 0; lon < longitudeCount; ++lon)
			{
				const uint32_t p0 = GetIndex(lat, lon);
				const uint32_t p1 = GetIndex(lat, lon + 1);
				const uint32_t p2 = GetIndex(lat + 1, lon);
				const uint32_t p3 = GetIndex(lat + 1, lon + 1);

				// 삼각형 1
				indices.push_back(p0);
				indices.push_back(p3);
				indices.push_back(p2);

				// 삼각형 2
				indices.push_back(p0);
				indices.push_back(p1);
				indices.push_back(p3);
			}
		}

		SetColliderType(ColliderType::SphereCollider);
		SetCollisionOffset(Vector3::Zero);
		SetCollisionRadius(0.5f);
	}

	void Actor::UseCamera()
	{
		if (layer != Layer::Player) { return; }

		// 1. 플레이어 기준 yaw 행렬 값 얻기
		float radianY = DirectX::XMConvertToRadians(transform.rotation.y);
		Renderer::Get().GetGraphics().GetCamera().SetPawnYawMatrix(DirectX::XMMatrixRotationY(radianY));

		// 2. 플레이어를 타겟으로 설정
		Renderer::Get().GetGraphics().GetCamera().SetTarget(transform.position.ToXMVECTOR());
	}

	Vector3 Actor::GetForwardVector() const
	{
		const float yawRad = DirectX::XMConvertToRadians(transform.rotation.y);
		return Monkey::Vector3(sinf(yawRad), 0.0f, cosf(yawRad));
	}

	Vector3 Actor::GetRightVector() const
	{
		const float yawRad = DirectX::XMConvertToRadians(transform.rotation.y);
		return Monkey::Vector3(cosf(yawRad), 0.0f, -sinf(yawRad));
	}
}