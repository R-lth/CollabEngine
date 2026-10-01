#include "Bullet.h"
#include "Level/InGame.h"

Bullet::Bullet() : MoveableActor(GameConfig::Bullet::HP, GameConfig::Bullet::Speed, 0.f)
{
	layer = Layer::GameObject;
	objType = GameObjectType::BULLET;

	//@todo
	/*color = Monkey::Color::Red;
	shape = Monkey::Geometry::CreateSphere(color);*/
	SetMeshSphere(Monkey::Color::Green);

	SetCollisionEnabled(true);
}

void Bullet::Update(float deltaTime)
{
	super::Update(deltaTime);

	transform.position += moveDir * speed * deltaTime;

	lifeTime += deltaTime;
	if (lifeTime >= GameConfig::Bullet::LifeTime)
	{
		Destroy();
	}
}

void Bullet::OnCollision(const std::shared_ptr<Monkey::Actor>& other)
{
	super::OnCollision(other);

	if (!CheckTeam(other))	return;

	auto target = std::dynamic_pointer_cast<MoveableActor>(other);
	if (target == nullptr)	return;

	int enemyHp = target->GetHP();
	target->TakeDamageAt(moveDir, hp);
	hp -= enemyHp;

	if (hp <= 0)
	{
		Destroy();
	}
}

void Bullet::SetMoveDirection(const Monkey::Vector3& dir)
{
	moveDir = dir;
	moveDir.y = 0.f;
	moveDir.Normalize();
}

void Bullet::ShootAt(GameObjectType shootTeam, Monkey::Vector3 spawnPos, Monkey::Vector3 targetPos)
{
	SetTeam(shootTeam);
	Shoot(spawnPos, targetPos);
}

void Bullet::Shoot(Monkey::Vector3 spawnPos, Monkey::Vector3 targetPos)
{
	std::cout << "Shoot" << '\n';

	std::cout << ownerLevel << std::endl;
	if (ownerLevel == nullptr)
	{
		//ownerLevel =
	}
	//SetMeshSphere(Monkey::Color::Green);
	SetMeshSphere(ownerLevel->currStageData.bulletColor);
	this->transform.scale = Monkey::Vector3(1.f, 1.f, 1.f) * ownerLevel->currStageData.bulletScale;

	hp = ownerLevel->currStageData.playerBulletHP;

	Monkey::Vector3 dir = targetPos - spawnPos;
	dir.Normalize();

	spawnPos += dir * GameConfig::Bullet::SpawnOffset;
	SetPosition(spawnPos);

	SetMoveDirection(dir);
}

void Bullet::SetTeam(GameObjectType team)
{
	switch (team)
	{
	case GameObjectType::PLAYER:
		hp = GameConfig::Player::BulletHP;
		break;
	case GameObjectType::ALLY:
		hp = GameConfig::Player::BulletHP;
		break;
	case GameObjectType::ENEMY:
		hp = GameConfig::Enemy::BulletHP;
		break;
	case GameObjectType::BULLET:
		break;
	case GameObjectType::none:
		break;
	default:
		break;
	}
	this->team = team;
}

bool Bullet::CheckTeam(const std::shared_ptr<Monkey::Actor>& other)
{
	if (team == GameObjectType::PLAYER)
	{
		if (other->GetLayer() != Layer::Enemy)
			return false;
	}
	else if (team == GameObjectType::ENEMY)
	{
		if (other->GetLayer() != Layer::Player)
			return false;
	}

	return true;
}