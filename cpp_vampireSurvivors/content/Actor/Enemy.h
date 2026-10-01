#pragma once
#include <Actor/MoveableActor.h>

class Enemy : public MoveableActor
{
	TYPE_DECLARATIONS(Enemy, MoveableActor)

public:
	Enemy();

public:
	void BeginPlay() override;
	void Update(float deltaTime) override;
	void OnCollision(const std::shared_ptr<Monkey::Actor>& other) override;

public:
	void OnDeath() override;

private:
	bool isDropItem = false;

private:
	void AnimHit(float deltaTime);
	void AnimDead(float deltaTime);

public:
	void ActivateEnemy(int newHP, float newSpeed, Monkey::Vector3 spawnPos, float newScale);

	void DeactivateEnemy();
};
