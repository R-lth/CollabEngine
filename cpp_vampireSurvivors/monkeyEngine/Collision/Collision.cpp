#include "Collision.h"
#include "Actor/Actor.h"

#include <algorithm>

namespace Monkey
{
	Collision::Collision()
	{
	}

	void Collision::ProcessCollision(const std::vector<std::shared_ptr<Actor>>& actors)
	{
		// 레벨에 액터가 없으면 함수 종료.
		if (actors.empty())
		{
			return;
		}

		std::vector<CollisionPair> collidedActorList;
		const int count = static_cast<int>(actors.size());

		// 충돌 검사
		for (int ix = 0; ix < count; ++ix)
		{
			std::shared_ptr<Actor> left = actors[ix];
			if (!left || !left->IsActive() || !left->HasCollision()) { continue; }

			for (int jx = ix + 1; jx < count; ++jx)
			{
				std::shared_ptr<Actor> right = actors[jx];
				if (!right || !right->IsActive() || !right->HasCollision()) { continue; }

				// todo. 다시 점검하기 // 같은 layer면 충돌 검사 x (예, 적 액터끼리는 충돌 안함)
				if (left->GetLayer() == right->GetLayer()) { continue; }

				// 충돌 검사
				switch (left->GetColliderType())
				{
				case Actor::ColliderType::AABB:
					switch (right->GetColliderType())
					{
					case Actor::ColliderType::AABB:
						if (CheckAABBAABB(left, right)) 
						{
							// 이벤트 발행할 목록에 액터 쌍으로 추가
							CollisionPair pair = {};
							pair.actor = left;
							pair.other = right;

							collidedActorList.emplace_back(pair);
						}
						break;
					case Actor::ColliderType::SphereCollider:
						if (CheckSphereAABB(right, left)) 
						{
							// 이벤트 발행할 목록에 액터 쌍으로 추가
							CollisionPair pair = {};
							pair.actor = left;
							pair.other = right;

							collidedActorList.emplace_back(pair);
						}
						break;
					}
					break;
				case Actor::ColliderType::SphereCollider:
					switch (right->GetColliderType())
					{
					case Actor::ColliderType::AABB:   
						if (CheckSphereAABB(left, right)) 
						{
							// 이벤트 발행할 목록에 액터 쌍으로 추가
							CollisionPair pair = {};
							pair.actor = left;
							pair.other = right;

							collidedActorList.emplace_back(pair);
						}
						break;
					case Actor::ColliderType::SphereCollider:
						if (CheckSphereSphere(left, right)) 
						{
							// 이벤트 발행할 목록에 액터 쌍으로 추가
							CollisionPair pair = {};
							pair.actor = left;
							pair.other = right;

							collidedActorList.emplace_back(pair);
						}
						break;
					}
					break;
				}
			}
		}

		// 충돌 발생한 액터 목록 확인
		if (collidedActorList.empty()) { return; }

		// 충돌한 액터에 이벤트 실행
		for (const CollisionPair& pair : collidedActorList)
		{
			// 이미 삭제되거나 비활성화된 액터는 제외
			if (!pair.actor->IsActive() || !pair.other->IsActive()) { continue; }
			if (!pair.actor->HasCollision() || !pair.other->HasCollision()) { continue; }

			// 충돌 이벤트 발행.
			pair.actor->OnCollision(pair.other);
			pair.other->OnCollision(pair.actor);
		}
	}

	bool Collision::CheckAABBAABB(const std::shared_ptr<Actor>& left, const std::shared_ptr<Actor>& right)
	{
		if (!left || !right)
		{
			return false;
		}

		if (!left->HasCollision() || !right->HasCollision())
		{
			return false;
		}

		// UI, Grid는 기본적으로 충돌 제외
		if (left->GetLayer() == Actor::Layer::UI || right->GetLayer() == Actor::Layer::UI)
		{
			return false;
		}

		if (left->GetLayer() == Actor::Layer::Grid || right->GetLayer() == Actor::Layer::Grid)
		{
			return false;
		}

		Actor::AABB leftBox = left->GetAABB();
		Actor::AABB rightBox = right->GetAABB();

		if (leftBox.max.x < rightBox.min.x || leftBox.min.x > rightBox.max.x)
		{
			return false;
		}

		if (leftBox.max.y < rightBox.min.y || leftBox.min.y > rightBox.max.y)
		{
			return false;
		}

		if (leftBox.max.z < rightBox.min.z || leftBox.min.z > rightBox.max.z)
		{
			return false;
		}

		return true;
	}

	bool Collision::CheckSphereAABB(const std::shared_ptr<Actor>& sphereActor, const std::shared_ptr<Actor>& boxActor)
	{
		if (sphereActor->GetColliderType() != Actor::ColliderType::SphereCollider ||
			boxActor->GetColliderType() != Actor::ColliderType::AABB)
		{
			return false;
		}

		Actor::SphereCollider sphere = sphereActor->GetSphereCollider();
		Actor::AABB box = boxActor->GetAABB();

		// 원과 가장 가까운 거리 // (min ~ max) 중에서 중점에서 가장 가까운 점을 반환 // std::clamp(sphere.center.x/*중점*/, box.min.x, box.max.x)
		Vector3 closestPoint = Vector3(
			std::min(box.min.x, std::max(box.max.x, sphere.center.x)), // std::min으로 box의 max를 넘지 못하고, std::max로 box의 min을 넘지 못함 
			std::min(box.min.y, std::max(box.max.x, sphere.center.y)),
			std::min(box.min.z, std::max(box.max.x, sphere.center.z))
		);

		// 원의 중점과 사각형 간의 거리 
		Vector3 v = Vector3(
			sphere.center.x - closestPoint.x,
			sphere.center.y - closestPoint.y,
			sphere.center.z - closestPoint.z
		);

		return v.Length() < sphere.radius;
	}

	bool Collision::CheckSphereSphere(const std::shared_ptr<Actor>& left, const std::shared_ptr<Actor>& right)
	{
		Actor::SphereCollider leftSphere = left->GetSphereCollider();
		Actor::SphereCollider rightSphere = right->GetSphereCollider();

		Vector3 v = Vector3(
			leftSphere.center.x - rightSphere.center.x, 
			leftSphere.center.y - rightSphere.center.y,
			leftSphere.center.z - rightSphere.center.z
		);
		float distance = v.Length();

		return distance < (leftSphere.radius + rightSphere.radius);
	}
}