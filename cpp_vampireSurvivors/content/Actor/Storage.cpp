#include "Storage.h"
#include "Player.h"
#include "Item.h"
#include "Level/InGame.h"

Storage::Storage()
{
	objType = GameObjectType::STORAGE;

	SetCollisionEnabled(true);

	transform.scale = Monkey::Vector3(2.f, 1.f, 2.f);

	//color = Monkey::Color::Yellow;
	color = Monkey::Color(1.f, 1.f, 0.f);

	SetMeshCone(color);
	transform.scale.x = 2.5f;
	transform.scale.y = 0.2f;
	transform.scale.z = 2.5f;
}

void Storage::OnCollision(const std::shared_ptr<Monkey::Actor>& other)
{
	super::OnCollision(other);

	if (other->GetLayer() != Layer::Player) return;
	auto player = std::dynamic_pointer_cast<Player>(other);

	if (player == nullptr) return;
	if (!player->HasItems()) return;
	if (!CanStoreItem()) return;
	player->MoveItemsToStorage(std::dynamic_pointer_cast<Storage>(shared_from_this()));
}

bool Storage::CanStoreItem() const
{
	return storageSlots.size() < maxItemCount;
}

int Storage::AddItem(const std::shared_ptr<Item>& item)
{
	if (!CanStoreItem())
		return -1;

	int index = static_cast<int>(storageSlots.size());

	StorageSlot slot;
	slot.item = item;
	slot.localOffset = Monkey::Vector3(0.f, GameConfig::Storage::StackStartHeight + index * GameConfig::Storage::StackSpacing, 0.f);

	storageSlots.push_back(slot);
	RefreshStorageItemEffects();
	if (ownerLevel)
	{
		std::cout << "CheckStageProgress Call\n";
		ownerLevel->CheckStageProgress();
	}

	return index;
}

Monkey::Vector3 Storage::GetStorageSlotWorldPosition(int index) const
{
	if (index < 0 || index >= storageSlots.size()) 		return GetPosition();

	return GetPosition() + storageSlots[index].localOffset;
}

void Storage::RefreshStorageItemEffects()
{
	bool isFull = !CanStoreItem();
	for (int i = 0; i < storageSlots.size(); ++i)
	{
		auto item = storageSlots[i].item.lock();

		if (item == nullptr)	continue;

		bool isTop = i == static_cast<int>(storageSlots.size()) - 1;

		item->SetTopStackItem(isTop && !isFull);
	}
}