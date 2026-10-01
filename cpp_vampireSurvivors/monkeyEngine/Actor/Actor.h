#pragma once

#include "../Engine/Engine.h"
#include "../Core/Core.h"
#include "../Core/MonkeyObject.h"
#include "../Math/Math.h"
#include "../Render/RenderTypes.h"

#include <memory>
#include <DirectXMath.h>

namespace Monkey
{
	struct Shape;
	class Level;

	class Monkey_API Actor : private MonkeyObject, public std::enable_shared_from_this<Actor>
	{
		TYPE_DECLARATIONS(Actor, MonkeyObject)

	public:
		enum class Layer
		{
			Player,
			Enemy,
			Grid,
			GameObject,
			UI
		};

		struct Transform
		{
			Vector3 position = Vector3(0.f, 0.f, 0.f);
			Vector3 rotation = Vector3::Zero;
			Vector3 scale = Vector3::One;
		};

		struct AABB
		{
			AABB() = default;
			~AABB() = default;

			// x 좌우, y 상하, z 전방
			Vector3 min = Vector3::Zero; // left, bottom, back
			Vector3 max = Vector3::Zero; // right, top, front
		};

		struct SphereCollider
		{
			Vector3 center = Vector3::Zero;
			float radius = 0.5f;
		};

		enum class ColliderType
		{
			None,
			AABB,
			SphereCollider
		};

	public:
		Actor() = default;
		virtual ~Actor() = default;

	public:
		virtual void BeginPlay();
		virtual void Update(float deltaTime);
		void Render();
		virtual void OnDestroyed();
		virtual void OnCollision(const std::shared_ptr<Actor>& other);

	public:
		void QuitGame();
		void Destroy(); // 액터를 레벨에서 제거할 때 사용
		
	public:
		// Getter/Setter
		inline bool HasBeanPlay() const { return hasBeganPlay; }
		inline bool IsActive()    const { return isActive && !hasExpired; }
		inline bool HasExpired()  const { return hasExpired; }
		
		inline Vector3 GetPosition() const { return transform.position; }
		void SetPosition(const Vector3& newPosition);

		template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Level, T>::value>>
		inline std::shared_ptr<T> GetOwner() 
		{ 
			return std::dynamic_pointer_cast<T>(owner.lock());
		} 
		
		inline void SetOwner(std::weak_ptr<Level> newOwner) { owner = newOwner; }

		int GetWidth() const { return Engine::Get().GetWidth(); }
		int GetHeight() const { return Engine::Get().GetHeight(); }

		Layer GetLayer();

		// Collision
		inline bool HasCollision() const { return hasCollision; }
		inline void SetCollisionEnabled(bool enabled) { hasCollision = enabled; }

		inline ColliderType GetColliderType() const { return colliderType; }

		AABB GetAABB() const;
		SphereCollider GetSphereCollider() const;
		
	protected:
		void SetGrid(const Color& xAxis, const Color& yAxis, const Color& zAxis, const Color& grid);
		void SetMeshCone(const Color& color);
		void SetMeshSphere(const Color& color);
		void UseCamera();

		Monkey::Vector3 GetForwardVector() const;
		Monkey::Vector3 GetRightVector() const;

	private:
		inline void SetColliderType(ColliderType type) { colliderType = type; }
		inline void SetCollisionOffset(const Vector3& offset) { collisionOffset = offset; } // 중점
		//AABB
		inline void SetCollisionSize(const Vector3& size) { collisionSize = size; }
		//SphereCollider
		inline void SetCollisionRadius(float radius) { collisionRadius = radius; }

	protected:
		bool hasBeganPlay = false;
		bool isActive = true;
		bool hasExpired = false;

		std::weak_ptr<Level> owner;

	protected:
		Layer layer = Layer::GameObject;
		Transform transform; // 변환

	protected:
		Primitive primitiveType = Primitive::TriangleList;
		std::vector<Vertex> vertices;
		std::vector<int> indices;
		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		
	protected:
		bool hasCollision = true;
		ColliderType colliderType = ColliderType::None;
		
		Vector3 collisionOffset = Vector3::Zero; // (공통) 중점
		Vector3 collisionSize = Vector3::One; // AABB 크기

		float collisionRadius = 0.5f;
	};
}



