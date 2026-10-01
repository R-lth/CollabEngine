#pragma once
#include <Actor/MoveableActor.h>
class Item;
struct StorageSlot
{
	std::weak_ptr<Item> item;
	Monkey::Vector3 localOffset;
};

class Storage :public MoveableActor
{
	TYPE_DECLARATIONS(Storage, MoveableActor)

public:
	Storage();
public:
	void OnCollision(const std::shared_ptr<Monkey::Actor>& other) override;

public:
	bool CanStoreItem() const;
	int AddItem(const std::shared_ptr<Item>& item);
	Monkey::Vector3 GetStorageSlotWorldPosition(int index) const;
	void RefreshStorageItemEffects();
	bool IsFull() const { return storageSlots.size() >= maxItemCount; }
	void SetMaxItemCount(int count) { maxItemCount = count; }
public:

	const int GetStoredItemCount() const { return static_cast<int>(storageSlots.size()); }
	void RemoveStoredItems() { storageSlots.clear(); };

public:

	int maxItemCount = 3;
	std::vector<StorageSlot> storageSlots;
};
