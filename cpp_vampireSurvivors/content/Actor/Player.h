#pragma once
#include <Actor/MoveableActor.h>
#include <functional>

class Storage;
class Item;
struct ItemSlot
{
	std::weak_ptr<Item> item;
	Monkey::Vector3 localOffset;
};
class Player : public MoveableActor
{
	TYPE_DECLARATIONS(Player, MoveableActor)
private:
	std::vector<ItemSlot> itemSlots;
public:
	Player();

public:
	void BeginPlay() override;
	void Update(float deltaTime) override;
	void OnDeath() override;
public:
	int AddCarriedItem(const std::shared_ptr<Item>& item);
	Monkey::Vector3 GetItemSlotWorldPosition(int slotIndex) const;
	void RefreshCarriedItemOffsets();
	void SavePlayerState(bool bShotgun) { this->bShotgun = bShotgun; }

public:
	int GetCarriedItemCount() const;
public:
	bool CanPlayerShoot() const { return canPlayerShoot; }
private:
	bool canPlayerShoot = true;
public:
	void MoveItemsToStorage(const std::shared_ptr<Storage>& storage);
#pragma region 이벤트
public:
	bool HasItems() const { return !itemSlots.empty(); }
	using ItemCountChangedEvent = std::function<void(int)>;
	void SetItemCountChangedEvent(ItemCountChangedEvent event);
private:
	ItemCountChangedEvent onItemCountChanged;
#pragma endregion

private:
	Monkey::Vector3 followOffsetItemPos;
	bool bShotgun = false;
};
