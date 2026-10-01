#include "Level.h"

namespace Monkey 
{
	void Level::ProcessAddAndDestroyActors()
	{
		using iterator = std::vector<std::shared_ptr<Actor>>::iterator;

		for (iterator it = actorList.begin(); it != actorList.end();) 
		{
			it = (*it)->HasExpired() ? actorList.erase(it) : next(it);
		}

		if (addRequestedActorList.empty()) { return; }

		for (const std::shared_ptr<Actor>& actor : addRequestedActorList) 
		{
			actorList.emplace_back(actor);
		}
		addRequestedActorList.clear();
	}

	void Level::OnInitialized()
	{
		hasInitialized = true;
	}

	void Level::BeginPlay() 
	{
		for (std::shared_ptr<Actor>& actor : actorList) 
		{
			if (actor->HasBeanPlay()) { continue; }
			actor->BeginPlay();
		}
	}

	void Level::Update(float deltaTime)
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			if (!actor->IsActive()) { continue; }
			actor->Update(deltaTime);
		}

		collision.ProcessCollision(actorList);
	}

	void Level::Render() 
	{
		for (std::shared_ptr<Actor>& actor : actorList)
		{
			if (!actor->IsActive()) { continue; }
			actor->Render();
		}
	}
}