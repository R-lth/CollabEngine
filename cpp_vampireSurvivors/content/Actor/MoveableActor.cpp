#include "MoveableActor.h"

void MoveableActor::SetTarget(const std::shared_ptr<Monkey::Actor>& target)
{
	targetActor = target;
}

void MoveableActor::SetOwnerLevel(InGame* level)
{
	ownerLevel = level;
}

void MoveableActor::OnDeath()
{
	std::cout
		<< "Before : "
		<< speed
		<< std::endl;
	std::cout << "Moveable Death\n";

	isDead = true;

	SetCollisionEnabled(false);

	deathTimer = 0.f;

	deathStartScale = transform.scale;
	deathStartPos = transform.position;
}

void MoveableActor::Update(float deltaTime)
{
	super::Update(deltaTime);
}

void MoveableActor::MoveToTarget(Monkey::Vector3 moveDir, float deltaTime)
{
	moveDir.Normalize();

	transform.position += moveDir * speed * deltaTime;
}

bool MoveableActor::CheckTargetInRange(const std::shared_ptr<Monkey::Actor>& target, Monkey::Vector3& outDirection, float detectRange)
{
	if (target == nullptr)
		return false;

	outDirection = target->GetPosition() - transform.position;
	outDirection.y = 0.f;

	float distanceSq = outDirection.x * outDirection.x + outDirection.z * outDirection.z;

	return distanceSq < detectRange * detectRange;
}

#pragma region //피격

void MoveableActor::TakeDamageAt(Monkey::Vector3 hitDir, int damage)
{
	_DamageAnim(hitDir);
	TakeDamage(damage);
}

void MoveableActor::TakeDamage(int damage)
{
	std::cout << "HP : " << hp;
	hp -= damage;
	std::cout << "-> HP : " << hp << '\n';

	if (hp <= 0)
	{
		OnDeath();
	}
}

void MoveableActor::_DamageAnim(Monkey::Vector3 hitDir)
{
	isHit = true;
	hitTimer = 0.f;
	//originColor = color;
	//color = hitColor;

	originScale = transform.scale;

	hitKnockbackDir = hitDir;
}

#pragma endregion