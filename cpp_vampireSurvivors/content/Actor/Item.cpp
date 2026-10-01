#include "Item.h"
#include "Player.h"
#include "Storage.h"
#include <iostream>
#include <cmath>

Item::Item() : MoveableActor()
{
	// 1. layer 설정
	// layer를 player로 설정해야 player 맞춰서 카메라가 회전됩니다. // (주의) 싱글 게임을 고려해, player 객체는 하나
	layer = Layer::GameObject;

	// 2. transform 설정
	//transform.position = Monkey::Vector3(2.f, 1.f, 10.f);
	this->transform.scale = Monkey::Vector3(1.f, 0.2f, 1.f);

	// 3. 기하 모양 및 색상 설정
	SetMeshSphere(Monkey::Color::White);

	// 4. 충돌 설정
	SetCollisionEnabled(true);
	//SetCollisionOffset(Monkey::Vector3(0.f, 0.f, 0.f));
}

void Item::Update(float deltaTime)
{
	super::Update(deltaTime);

	if (state == ItemState::StackedBehindPlayer || state == ItemState::StackedInStorage)
	{
		Monkey::Vector3 pos;
		if (state == ItemState::StackedBehindPlayer)
		{
			auto player = followPlayer.lock();
			if (player == nullptr) return;
			pos = player->GetItemSlotWorldPosition(slotIndex);
		}
		else if (state == ItemState::StackedInStorage)
		{
			auto storage = followStorage.lock();
			if (storage == nullptr) return;
			pos = storage->GetStorageSlotWorldPosition(storageSlotIndex);
		}

		if (isTopStackItem)
		{
			bounceTimer += deltaTime;

			float bounce = abs(sinf(bounceTimer * GameConfig::Item::BounceSpeed)) * GameConfig::Item::BounceHeight;

			pos.y += bounce;
		}

		SetPosition(pos);

		return;
	}

	AnimDrop(deltaTime);
	AnimMoveTo(deltaTime);
}

void Item::OnCollision(const std::shared_ptr<Monkey::Actor>& other)
{
	super::OnCollision(other);

	if (other->GetLayer() == Layer::Player)
	{
		{
			std::cout << this << std::endl;
			std::cout << this->hasCollision << std::endl;
		}

		auto player = std::dynamic_pointer_cast<Player>(other);

		if (player == nullptr) 	return;

		SetCollisionEnabled(false);

		followPlayer = player;
		slotIndex = player->AddCarriedItem(std::dynamic_pointer_cast<Item>(shared_from_this()));

		moveTimer = 0.f;
		moveStartPos = GetPosition();
		moveTargetPos = player->GetItemSlotWorldPosition(slotIndex);

		state = ItemState::MovingToPlayer;
	}
}

void Item::StartDropAnimation(const Monkey::Vector3& startPos, const Monkey::Vector3& hitDir)
{
	isDropAnimating = true;
	dropTimer = 0.f;

	dropStartPos = startPos;

	dropDir = hitDir;
	dropDir.y = 0.f;
	dropDir.Normalize();

	SetPosition(dropStartPos);
}

void Item::ClearFromStorage()
{
	SetCollisionEnabled(false);
	SetTopStackItem(false);

	followStorage.reset();
	storageSlotIndex = -1;

	Destroy();
}

void Item::AnimDrop(float deltaTime)
{
	if (isDropAnimating)
	{
		dropTimer += deltaTime;

		float t = dropTimer / dropDuration;
		if (t > 1.f)
			t = 1.f;

		Monkey::Vector3 pos = dropStartPos + dropDir * dropDistance * t;
		pos.y = dropStartPos.y + jumpHeight * 4.f * t * (1.f - t);

		SetPosition(pos);

		if (t >= 1.f)
		{
			isDropAnimating = false;
		}
	}
}

void Item::AnimMoveTo(float deltaTime)
{
	if (state == ItemState::MovingToPlayer ||
		state == ItemState::MovingToStorage)
	{
		moveTimer += deltaTime;

		float t = moveTimer / moveDuration;
		if (t > 1.f) t = 1.f;

		// 플레이어 이동에 맞춰 목표 위치 계속 갱신
		if (state == ItemState::MovingToPlayer)
		{
			auto player = followPlayer.lock();

			if (state == ItemState::MovingToPlayer && player != nullptr)
			{
				moveTargetPos = player->GetItemSlotWorldPosition(slotIndex);
			}
		}
		else if (state == ItemState::MovingToStorage)
		{
			auto storage = followStorage.lock();

			if (storage != nullptr)
			{
				moveTargetPos = storage->GetStorageSlotWorldPosition(storageSlotIndex);
			}
		}

		Monkey::Vector3 pos = moveStartPos + (moveTargetPos - moveStartPos) * t;
		pos.y += moveJumpHeight * 4.f * t * (1.f - t);

		SetPosition(pos);

		if (t >= 1.f)
		{
			SetPosition(moveTargetPos);

			if (state == ItemState::MovingToPlayer)
				state = ItemState::StackedBehindPlayer;
			else
				state = ItemState::StackedInStorage;
		}

		return;
	}
}

void Item::SetFollowOffset(const Monkey::Vector3& offset)
{
	followOffset = offset;
}

void Item::SetTopStackItem(bool value)
{
	isTopStackItem = value;

	if (value == false)
	{
		bounceTimer = 0.f;
	}
}

void Item::MoveToStorage(const std::shared_ptr<Storage>& storage, int slotIndex)
{
	followStorage = storage;
	storageSlotIndex = slotIndex;

	moveTimer = 0.f;
	moveStartPos = GetPosition();
	moveTargetPos = storage->GetStorageSlotWorldPosition(slotIndex);

	state = ItemState::MovingToStorage;
}