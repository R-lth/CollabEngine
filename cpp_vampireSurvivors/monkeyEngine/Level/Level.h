#pragma once

#include "../Core/Core.h"
#include "../Actor/Actor.h"
#include "../Collision/Collision.h"

#include <vector>
#include <memory>

namespace Monkey
{
	class Monkey_API Level : public std::enable_shared_from_this<Level>
	{
		friend class Engine;

	public:
		Level() = default;
		virtual ~Level() = default;

	protected:
		template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> SpawnActor(Args&&... args) 
		{
			std::shared_ptr<T> newActor = std::make_shared<T>(std::forward<Args>(args)...);
			addRequestedActorList.emplace_back(newActor);
			newActor->SetOwner(shared_from_this()); // todo. shared_from_this
			return newActor;
		}
		void ProcessAddAndDestroyActors();
		bool HasInitialized() const { return hasInitialized; }

		virtual void OnInitialized(); 
		virtual void BeginPlay();
		virtual void Update(float deltaTime);
		virtual void Render();

	private:
		bool IsInitialized() const { return true; }

	private:
		bool hasInitialized = false;

		std::vector<std::shared_ptr<Actor>> actorList;
		std::vector<std::shared_ptr<Actor>> addRequestedActorList;

	private:
		Collision collision;
	};
}
