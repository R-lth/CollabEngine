#pragma once
#include <Actor/Actor.h>
//#include "../Level/InGame.h"
#include<iostream>

#include "GameConfig.h"

enum class GameObjectType
{
	PLAYER,
	ALLY,
	ENEMY,
	BULLET,
	STORAGE,
	none
};

class InGame;
class MoveableActor :public Monkey::Actor
{
	TYPE_DECLARATIONS(MoveableActor, Actor)

public:
	GameObjectType objType;

protected:
	InGame* ownerLevel = nullptr;

public:
	MoveableActor() : MoveableActor(0, 0, 0)
	{
	}

	MoveableActor(int hp, float speed, float detectRange) : Actor()
	{
		this->Maxhp = hp;
		this->hp = hp;
		this->speed = speed;
		this->detectRange = detectRange;
	}

protected:
	std::shared_ptr<Monkey::Actor> targetActor;
	int Maxhp;
	int hp;
	float speed;
	float detectRange;
	Monkey::Color color;

protected:
	bool isHit = false;

	float hitTimer = 0.f;
	float hitDuration = 0.3f;

	Monkey::Color originColor;
	Monkey::Color hitColor = Monkey::Color::Red;

	Monkey::Vector3 originScale;
	Monkey::Vector3 hitKnockbackDir;

	float hitScalePower = GameConfig::Enemy::hitScalePower;
	float knockbackPower = GameConfig::Enemy::knockbackPower;

protected:
	bool isDead = false;

	float deathTimer = 0.f;
	float deathTimer_Quit = 0.f;
	float deathDuration = 1.f;
	float deathDuration_Quit = 1.5f;

	Monkey::Vector3 deathStartScale;
	Monkey::Vector3 deathStartPos;

public:
	int GetMaxHP() const { return Maxhp; }
	int GetHP() const { return hp; }
	float GetSpeed() const { return speed; }
	Monkey::Vector3 GetScale() const { return originScale; }
	bool IsDead() const { return isDead; }

public:
	void Update(float deltaTime) override;

protected:
	void MoveToTarget(Monkey::Vector3 moveDir, float deltaTime);
	bool CheckTargetInRange(const std::shared_ptr<Monkey::Actor>& target, Monkey::Vector3& outDirection, float detectRange);

public:
	void TakeDamageAt(Monkey::Vector3 hitDir, int damage);
	void TakeDamage(int damage);

private:
	void _DamageAnim(Monkey::Vector3 hitDir);

public:
	void SetTarget(const std::shared_ptr<Monkey::Actor>& target);

public:
	void SetOwnerLevel(InGame* level);

public:
	virtual void OnDeath();
};
