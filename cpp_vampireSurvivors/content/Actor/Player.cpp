#include "Player.h"
#include <Core/Input.h>
#include <Render/RenderTypes.h>
#include <Render/Renderer.h>
#include <Level/InGame.h>
#include "Game/VampireSurvivors.h"
#include "Item.h"
#include "Storage.h"

Player::Player() : MoveableActor(GameConfig::Player::HP, GameConfig::Player::Speed, GameConfig::Player::DetectRange)
{
	// 1. layer 설정
	// layer를 player로 설정해야 player 맞춰서 카메라가 회전됩니다. // (주의) 싱글 게임을 고려해, player 객체는 하나
	layer = Layer::Player;

	//// 2. transform 설정
	//// transform

	//// 3. 기하 모양 및 색상 설정
	color = Monkey::Color(0.f, 1.f, 0.f);
	originColor = color;
	//shape = Monkey::Geometry::CreateSphere(color);

	//// 4. 충돌 설정
	//SetCollisionEnabled(true);
	//SetCollisionOffset(Monkey::Vector3(0.f, 0.f, 0.f));

	// 2. transform 설정
	transform.scale = Monkey::Vector3(2.f, 2.f, 2.f);

	// 3. 기하 모양(시계 방향) 및 색상 설정
	SetMeshCone(color);

	// 4. 충돌 설정
	SetCollisionEnabled(true);

	// offsetItem
	followOffsetItemPos = Monkey::Vector3(GameConfig::Player::followOffsetX, GameConfig::Player::followOffsetY, GameConfig::Player::followOffsetZ);
}

void Player::BeginPlay()
{
	super::BeginPlay();
}

void Player::Update(float deltaTime)
{
	super::Update(deltaTime);

	if (isDead == true)
	{
		//std::cout << "1";
		deathTimer_Quit += deltaTime;
		if (deathTimer_Quit >= deathDuration_Quit)
		{
			//	std::cout << "2";

			QuitGame();
		}
	}
	//_아이템

	// 1. 마우스 좌우 이동량(delta.x)으로 캐릭터의 Y축 회전(Yaw) 변경 // 도리도리
	if (Monkey::Input::Get().GetMouseButton(Monkey::Input::MouseButton::Middle))
	{
		POINT delta = Monkey::Input::Get().GetMouseDelta(); // 마우스 이동향
		float sensitivity = 0.2f; // 마우스 회전 감도

		transform.rotation.y += static_cast<float>(delta.x) * sensitivity;
	}

	// 이동량
	float moveDirX = 0.0f;
	float moveDirZ = 0.0f;

	if (Monkey::Input::Get().GetKey('W') || (Monkey::Input::Get().GetKey(VK_UP))) {
		moveDirZ += 1.f;
	}
	if (Monkey::Input::Get().GetKey('S') || Monkey::Input::Get().GetKey(VK_DOWN)) { moveDirZ -= 1.f; }
	if (Monkey::Input::Get().GetKey('D') || Monkey::Input::Get().GetKey(VK_RIGHT)) { moveDirX += 1.f; }
	if (Monkey::Input::Get().GetKey('A') || Monkey::Input::Get().GetKey(VK_LEFT)) { moveDirX -= 1.f; }

	// 회전 기준 // 플레이어가 바라보는 방향으로 이동
	Monkey::Vector3 moveDir = Monkey::Vector3(
		GetForwardVector().x * moveDirZ + GetRightVector().x * moveDirX, // 전방 입력량만큼 좌우 벡터 + 좌우 입력량만큼 전방 벡터 = 새로운 좌우 벡터
		0.f,
		GetForwardVector().z * moveDirZ + GetRightVector().z * moveDirX
	);

	// 2. 이동
	float speed = 5.0f;
	transform.position.z += moveDir.z * speed * deltaTime;
	transform.position.x += moveDir.x * speed * deltaTime;

	// 3. 카메라 셋팅
	UseCamera();

	// 4. 공격
	if (bShotgun && Monkey::Input::Get().GetKeyDown(VK_SPACE))
	{
		if (!ownerLevel->currStageData.canShoot) return;

		ownerLevel->PlayerAttack();
	}

	// 6. (임시) 게임 종료
	if (Monkey::Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
	}
	if (Monkey::Input::Get().GetKeyDown('R'))
	{
		//@todo : 재시작
	}
	if (Monkey::Input::Get().GetKeyDown('L'))
	{
		SetPosition(Monkey::Vector3(0, 0, 0));
	}
}

void Player::OnDeath()
{
	deathTimer = 0;

	super::OnDeath();
	std::cout << "PlayerOnDeath" << '\n';
	SetMeshSphere(Monkey::Color::Red);
}

int Player::AddCarriedItem(const std::shared_ptr<Item>& item)
{
	ItemSlot slot;

	int index = static_cast<int>(itemSlots.size());

	slot.item = item;
	slot.localOffset = Monkey::Vector3(
		GameConfig::Player::followOffsetX,
		GameConfig::Player::followOffsetY + index * GameConfig::Player::StackSpacing,
		GameConfig::Player::followOffsetZ
	);

	itemSlots.push_back(slot);

	RefreshCarriedItemOffsets();

	//이벤트 호출
	if (onItemCountChanged)
	{
		onItemCountChanged(static_cast<int>(itemSlots.size()));
	}

	return index;
}

Monkey::Vector3 Player::GetItemSlotWorldPosition(int slotIndex) const
{
	if (slotIndex < 0 || slotIndex >= itemSlots.size())
	{
		return GetPosition();
	}

	Monkey::Vector3 local = itemSlots[slotIndex].localOffset;

	// 현재는 회전 미반영. 그래도 슬롯 구조는 완성.
	return GetPosition() + local;
}

void Player::RefreshCarriedItemOffsets()
{
	for (int i = 0; i < itemSlots.size(); ++i)
	{
		auto item = itemSlots[i].item.lock();

		if (item == nullptr)
			continue;

		itemSlots[i].localOffset.y = GameConfig::Player::followOffsetY + i * GameConfig::Player::StackSpacing;

		item->SetTopStackItem(i == itemSlots.size() - 1);
	}
}

int Player::GetCarriedItemCount() const
{
	return static_cast<int>(itemSlots.size());
}

void Player::MoveItemsToStorage(const std::shared_ptr<Storage>& storage)
{
	if (storage == nullptr)
		return;

	for (int i = static_cast<int>(itemSlots.size()) - 1; i >= 0; --i)
	{
		auto item = itemSlots[i].item.lock();

		if (item == nullptr)	continue;
		if (!storage->CanStoreItem())	break;

		int storageSlotIndex = storage->AddItem(item);

		if (storageSlotIndex == -1)	continue;

		//if (item->itemType == ItemType::MEAT)
		{
			item->MoveToStorage(storage, storageSlotIndex);
		}
		itemSlots.erase(itemSlots.begin() + i);
	}

	RefreshCarriedItemOffsets();

	if (onItemCountChanged)
	{
		onItemCountChanged(static_cast<int>(itemSlots.size()));
	}
}

void Player::SetItemCountChangedEvent(ItemCountChangedEvent event)
{
	onItemCountChanged = event;
}