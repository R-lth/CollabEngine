#pragma once

#include "Core/Core.h"
#include <vector>
#include <memory>

namespace Monkey
{
	class Actor;

	class Monkey_API Collision
	{
		// 충돌 이벤트 발생을 위해 액터를 저장할 구조체.
		struct CollisionPair
		{
			std::shared_ptr<Actor> actor;
			std::shared_ptr<Actor> other;
		};

	public:
		Collision();
		~Collision() = default;

		void ProcessCollision(const std::vector<std::shared_ptr<Actor>>& actors);

	private:
		//bool Test(const std::shared_ptr<Actor>& left, const std::shared_ptr<Actor>& right);
		bool CheckAABBAABB(const std::shared_ptr<Actor>& left, const std::shared_ptr<Actor>& right);
		bool CheckSphereAABB(const std::shared_ptr<Actor>& sphereActor, const std::shared_ptr<Actor>& boxActor);
		bool CheckSphereSphere(const std::shared_ptr<Actor>& left, const std::shared_ptr<Actor>& right);
	};
}