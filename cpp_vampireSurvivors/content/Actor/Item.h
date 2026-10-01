#pragma once
#include <Actor/MoveableActor.h>
enum class ItemType
{
	MEAT,
	WOOD,
	Gold,

	none
};
enum class ItemState
{
	Dropped,
	MovingToPlayer,
	StackedBehindPlayer,
	MovingToStorage,
	StackedInStorage
};
class Storage;
class Player;
class Item : public MoveableActor
{
	TYPE_DECLARATIONS(Item, MoveableActor)
public:
	Item();

public:
	ItemType itemType = ItemType::MEAT;

public:
	bool isDropAnimating = false;

	float dropTimer = 0.f;
	float dropDuration = 0.4f;

	float dropDistance = .2f;
	float jumpHeight = 1.2f;

	Monkey::Vector3 dropStartPos;
	Monkey::Vector3 dropDir;

public:
	void Update(float deltaTime) override;
	void OnCollision(const std::shared_ptr<Monkey::Actor>& other) override;

public:
	void AnimDrop(float deltaTime);
	void StartDropAnimation(const Monkey::Vector3& startPos, const Monkey::Vector3& hitDir);

public:
	void ClearFromStorage();

#pragma region 아이템 이동

private:
	ItemState state = ItemState::Dropped;

	Monkey::Vector3 moveStartPos;
	Monkey::Vector3 moveTargetPos;

	float moveTimer = 0.f;
	float moveDuration = 1.f;
	float moveJumpHeight = 2.2f;

public:
	void AnimMoveTo(float deltaTime);

public:
	void SetFollowOffset(const Monkey::Vector3& offset);

#pragma endregion

#pragma region //엔진에 부모 없어 임시로

	Monkey::Vector3 followOffset;

#pragma endregion

public:
	void SetTopStackItem(bool value);

private:
	bool isTopStackItem = false;
	float bounceTimer = 0.f;

private:
	std::weak_ptr<Player> followPlayer;
	int slotIndex = -1;

public:
	void MoveToStorage(const std::shared_ptr<Storage>& storage, int slotIndex);
	std::weak_ptr<Storage> followStorage;
	int storageSlotIndex = -1;
};
