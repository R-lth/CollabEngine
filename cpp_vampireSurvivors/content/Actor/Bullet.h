#pragma once
#include <Actor/MoveableActor.h>

class Bullet :public MoveableActor
{
	TYPE_DECLARATIONS(Bullet, MoveableActor)

public:
	Bullet();
public:
	virtual void Update(float deltaTime) override;
	void OnCollision(const std::shared_ptr<Monkey::Actor>& other) override;

public:

	void SetMoveDirection(const Monkey::Vector3& dir);
	void Shoot(Monkey::Vector3 spawnPos, Monkey::Vector3 targetPos);
	void ShootAt(GameObjectType shootTeam, Monkey::Vector3 spawnPos, Monkey::Vector3 targetPos);

private:
	Monkey::Vector3 moveDir = Monkey::Vector3::Zero;
	float lifeTime = 0.f;
	float damage = 0.f;

private:
	GameObjectType team = GameObjectType::PLAYER;
public:
	void SetTeam(GameObjectType team);
	bool CheckTeam(const std::shared_ptr<Monkey::Actor>& other);
};
