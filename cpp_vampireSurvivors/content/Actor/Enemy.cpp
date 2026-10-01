#include "Enemy.h"
#include <Level/InGame.h>

#include <iostream>

Enemy::Enemy() :MoveableActor(GameConfig::Enemy::HP, GameConfig::Enemy::Speed, GameConfig::Enemy::DetectRange)
{
	// 1. layer 설정
	// layer를 player로 설정해야 player 맞춰서 카메라가 회전됩니다. // (주의) 싱글 게임을 고려해, player 객체는 하나
	layer = Layer::Enemy;

	// 2. transform 설정
	//transform.position = Monkey::Vector3(10.f, 2.f, 10.f);
	//transform.scale = Monkey::Vector3(1.f, 1.f, 1.f) * ownerLevel->currStageData.enemyScale;;
	transform.scale = Monkey::Vector3(2.f, 2.f, 2.f);

	// (임시) 테스트용
	// 3. 기하 모양(시계 방향) 및 색상 설정
	//color =/* Monkey::Color::Black; */ ownerLevel->currStageData.enemyColor;
	color = Monkey::Color::Black;
	originColor = color;
	SetMeshSphere(color);

	// 4. 충돌 설정
	SetCollisionEnabled(true);
}

void Enemy::BeginPlay()
{
	super::BeginPlay();
}

void Enemy::Update(float deltaTime)
{
	super::Update(deltaTime);

	if (!isActive) 		return;

	AnimHit(deltaTime);
	AnimDead(deltaTime);

	if (targetActor == nullptr) {
		return;
	}

	Monkey::Vector3 moveDir;
	if (CheckTargetInRange(targetActor, moveDir, detectRange))
	{
		MoveToTarget(moveDir, deltaTime);
	}
}

void Enemy::OnCollision(const std::shared_ptr<Monkey::Actor>& other)
{
	super::OnCollision(other);

	if (isDead)
		return;

	if (other->GetLayer() == Layer::Player)
	{
		auto player = std::dynamic_pointer_cast<Player>(other);

		if (player != nullptr)
		{
			player->TakeDamage(player->GetHP());
		}
	}
}

void Enemy::OnDeath()
{
	super::OnDeath();
}

void Enemy::AnimHit(float deltaTime)
{
	if (isHit)
	{
		hitTimer += deltaTime;

		int blink = static_cast<int>(hitTimer * 20.f);

		SetMeshSphere((blink % 2 == 0) ? hitColor : originColor);

		float t = hitTimer / hitDuration;
		float scaleValue = 1.f + (1.f - t) * (hitScalePower - 1.f);
		transform.scale = originScale * scaleValue;
		transform.position += hitKnockbackDir * knockbackPower * deltaTime;

		if (hitTimer >= hitDuration)
		{
			isHit = false;
			if (GetHP() <= 0)
			{
				SetMeshSphere(Monkey::Color::White);
			}
			else
			{
				SetMeshSphere(originColor);
			}

			transform.scale = originScale;
		}
	}
}

void Enemy::AnimDead(float deltaTime)
{
	if (isDead)
	{
		deathTimer += deltaTime;

		float t = deathTimer / deathDuration;

		if (t > 1.f)
		{
			t = 1.f;
		}

		transform.scale.x = deathStartScale.x;
		transform.scale.z = deathStartScale.z;

		float scaleY = deathStartScale.y * (1.f - t);
		transform.scale.y = scaleY;

		transform.position.y = deathStartPos.y - (deathStartScale.y - scaleY) * 0.5f;
		//transform.rotation.z = 90.f * t;

		if (!isDropItem && scaleY <= GameConfig::Enemy::DropScale)
		{
			isDropItem = true;
			///Monkey::Vector3 dropPos = GetPosition();
			//dropPos.y += 0.3f;

			Monkey::Vector3 dropPos = deathStartPos; // 중점 위치로 줘야 함

			if (GetOwner<InGame>())
			{
				GetOwner<InGame>()->SpawnItem(ItemType::MEAT, dropPos, hitKnockbackDir);
			}

			//ownerLevel->SpawnItem(ItemType::MEAT, dropPos, hitKnockbackDir);
		}

		if (deathTimer >= deathDuration)
		{
			//DeactivateEnemy();

			if (GetOwner<InGame>())
			{
				std::cout << this << "<< Enemy Deactivate\n" << this->GetSpeed() << "\n";

				GetOwner<InGame>()->RespawnEnemy(shared_from_this());
			}

			return;
		}

		return;
	}
}

void Enemy::ActivateEnemy(int newHP, float newSpeed, Monkey::Vector3 spawnPos, float newScale)
{
	Maxhp = newHP;
	hp = newHP;
	speed = newSpeed;

	isActive = true;
	isDead = false;
	isHit = false;
	isDropItem = false;

	deathTimer = 0.f;
	hitTimer = 0.f;

	SetPosition(spawnPos);

	SetCollisionEnabled(true);

	if (GetOwner<InGame>())
	{
		color = ownerLevel->currStageData.enemyColor;
		originColor = color;
		SetMeshSphere(color);

		transform.scale = Monkey::Vector3(1.f, 1.f, 1.f) * newScale;

		SetTarget(GetOwner<InGame>()->GetPlayer());
	}
}

void Enemy::DeactivateEnemy()
{
	isActive = false;
	isDead = false;
	isHit = false;
	isDropItem = false;

	SetCollisionEnabled(false);

	SetPosition(Monkey::Vector3(9999.f, 9999.f, 9999.f));
}