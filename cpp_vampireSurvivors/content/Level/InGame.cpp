#include "InGame.h"
#include <Core/Input.h>
#include "Game/VampireSurvivors.h"

#pragma region Init / Spawn

InGame::InGame() : Level()
{
}

void InGame::OnInitialized()
{
	Level::OnInitialized();

	srand((unsigned int)time(nullptr));

	// 1. 축과 그리드
	SpawnActor<GridActor>();

	switch (ttype)
	{
	case TESTT::TEST:
#pragma region //엔진 테스트용

#pragma endregion
		break;

	case TESTT::none:
	{
		// 2. player
		player = SpawnActor<Player>();
		player->SetOwnerLevel(this);

		//player->GetOwner();

		// 3. enemy
		SpawnMoveableActor<Enemy>(enemies, GameConfig::Enemy::Count, GameConfig::Enemy::StartRadius, GameConfig::Enemy::OffsetRadius);
		SpawnMoveableActor<Item>(items, GameConfig::Item::Count, GameConfig::Item::StartRadius, GameConfig::Item::OffsetRadius);
		SpawnMoveableActor<Storage>(storages, GameConfig::Storage::Count, GameConfig::Storage::StartRadius, GameConfig::Storage::OffsetRadius);

		// UI	//이벤트 등록
		auto ui = SpawnActor<ItemCountUI>();
		player->SetItemCountChangedEvent([ui](int count) {ui->SetCount(count); });

		//currStageData = GameConfig::Stage::Datas[0];
		ApplyStageData();
	}
	break;
	default:
		break;
	}
}

void InGame::Update(float deltaTime)
{
	Level::Update(deltaTime);

	if (bEnter)
	{
		const VampireSurvivors::GameState state = VampireSurvivors::Get().LoadGameState();
		player->SavePlayerState(state.weaponUnlock);

		for (std::shared_ptr<Storage>& storage : storages)
		{
			if (!storage || storage->storageSlots.empty()) { continue; }

			for (StorageSlot& slot : storage->storageSlots)
			{
				std::shared_ptr<Item> item = slot.item.lock();

				if (!item)
				{
					continue;
				}

				item->ClearFromStorage();
			}

			storage->storageSlots.clear();
		}

		bEnter = false;
	}

	if (Monkey::Input::Get().GetKeyDown('M'))
	{
		SaveGameInfo();
		bEnter = true;
		VampireSurvivors::Get().ToggleLevel();
	}

	if (Monkey::Input::Get().GetKeyDown(VK_ESCAPE))
	{
		VampireSurvivors::Get().Quit();
	}
}

void InGame::SaveGameInfo() const
{
	VampireSurvivors::GameState state = VampireSurvivors::Get().LoadGameState();

	if (player)
	{
		state.itemCount = player->GetCarriedItemCount();
	}

	int storageSum = 0;
	for (const std::shared_ptr<Storage> storage : storages)
	{
		if (storage)
		{
			storageSum += storage->GetStoredItemCount();
		}
	}

	if (state.storageCount != storageSum) { state.storageCount = storageSum; }
	VampireSurvivors::Get().SaveGameState(state);
}

void InGame::SpawnEnemy(int count, float start, float offset)
{
	for (int i = 0; i < count; i++)
	{
		auto enemy = SpawnActor<Enemy>();
		enemy->SetTarget(player);
		enemy->SetPosition(_GetRandomSpawnPoslikeDoughnut(player->GetPosition().x, player->GetPosition().z, start, offset));
	}
}

template<typename T>
void InGame::SpawnMoveableActor(int count, float start, float offset)
{
	for (int i = 0; i < count; i++)
	{
		auto actor = SpawnActor<T>();
		actor->SetTarget(player);
		actor->SetPosition(_GetRandomSpawnPoslikeDoughnut(player->GetPosition().x, player->GetPosition().z, start, offset));
	}
}

template<typename T>
void InGame::SpawnMoveableActor(std::vector<std::shared_ptr<T>>& actorList, int count, float start, float offset)
{
	if (count == 0) return;
	for (int i = 0; i < count; i++)
	{
		auto actor = SpawnActor<T>();
		actor->SetTarget(player);
		actor->SetPosition(_GetRandomSpawnPoslikeDoughnut(player->GetPosition().x, player->GetPosition().z, start, offset));

		actor->SetOwnerLevel(this);

		actorList.push_back(actor);
	}
}

Monkey::Vector3 InGame::_GetRandomSpawnPoslikeDoughnut(float _x, float _z, float start, float offset)
{
	/*float centerX = 10.f;
	float centerZ = 10.f;*/

	float centerX = _x;
	float centerZ = _z;

	float angle = static_cast<float>(rand()) / RAND_MAX * 2.f * 3.141592f;
	float radius = start + static_cast<float>(rand()) / RAND_MAX * offset;

	float x = centerX + cosf(angle) * radius;
	float z = centerZ + sinf(angle) * radius;

	return  Monkey::Vector3(x, 1.f, z);
}

void InGame::SpawnItem(ItemType itemType, Monkey::Vector3 position, Monkey::Vector3  hitKnockbackDir)
{
	std::shared_ptr<Item> item = SpawnActor<Item>();

	item->SetPosition(position);
	item->itemType = itemType;
	item->StartDropAnimation(position, hitKnockbackDir);
}

#pragma endregion

#pragma region Attack / Bullet

std::shared_ptr<Enemy> InGame::FindNearestEnemy()
{
	std::shared_ptr<Enemy> nearestEnemy = nullptr;

	float nearestDistanceSq = FLT_MAX;

	for (auto enemy : enemies)
	{
		if (enemy == nullptr || enemy->IsDead())
			continue;

		Monkey::Vector3 dir = enemy->GetPosition() - player->GetPosition();

		dir.y = 0.f;

		float distanceSq = dir.x * dir.x + dir.z * dir.z;

		if (distanceSq < nearestDistanceSq)
		{
			nearestDistanceSq = distanceSq;
			nearestEnemy = enemy;
		}
	}

	float detectRangeSq = GameConfig::Player::DetectRange * GameConfig::Player::DetectRange;
	if (nearestDistanceSq >= detectRangeSq)
	{
		std::cout << detectRangeSq << ": too fall\n";

		//화면 밖에 있을 수도 있으니까, 제일 가까워도, 화면밖이면 공격 안할래
		return nullptr;
	}

	if (nearestEnemy->IsDead())
	{
		std::cout << detectRangeSq << ": dead \n";

		return nullptr;
	}

	return nearestEnemy;
}

void InGame::PlayerAttack()
{
	auto enemy = FindNearestEnemy();

	if (enemy == nullptr)
		return;

	auto bullet = SpawnActor<Bullet>();
	bullet->SetOwnerLevel(this); // 임시추가
	bullet->ShootAt(GameObjectType::PLAYER, player->GetPosition(), enemy->GetPosition());
}

void InGame::RespawnEnemy(const std::shared_ptr<Monkey::Actor>& enemyActor)
{
	auto enemy = std::dynamic_pointer_cast<Enemy>(enemyActor);

	if (enemy == nullptr)
		return;

	Monkey::Vector3 spawnPos = _GetRandomSpawnPoslikeDoughnut(player->GetPosition().x, player->GetPosition().z, GameConfig::Enemy::ReSpwanRadius, 1.f);

	//std::cout << "\n" << "  enemy->GetSpeed() : " << enemy->GetSpeed() << " " << "\n";
	//enemy->ActivateEnemy(enemy->GetMaxHP(), enemy->GetSpeed(), spawnPos, enemy->GetScale());

	enemy->ActivateEnemy(currStageData.enemyHP, currStageData.enemySpeed, spawnPos, currStageData.enemyScale);
}

void InGame::CheckStageProgress()
{
	//	std::cout << "CheckStageProgress Enter\n";
	for (auto& storage : storages)
	{
		if (storage == nullptr)
			continue;

		if (!storage->IsFull())
			return;
	}

	AdvanceStage();
}

void InGame::AdvanceStage()
{
	//std::cout << "AdvanceStage!\n";
	currentStageIndex++;
	ApplyStageData();

	// 스테이지 올라가면 적 다시 활성화
	if (currentStageIndex == 1)
	{
		SpawnMoveableActor<Enemy>(enemies, GameConfig::Enemy::addCount, GameConfig::Enemy::ReSpwanRadius, 1.f);
	}
}

GameConfig::Stage::StageData InGame::GetStageData(int stageIndex)
{
	if (stageIndex < GameConfig::Stage::FixedStageCount)
	{
		return GameConfig::Stage::Datas[stageIndex];
	}

	GameConfig::Stage::StageData base =
		GameConfig::Stage::Datas[GameConfig::Stage::FixedStageCount - 1];

	int extraLevel =
		stageIndex - GameConfig::Stage::FixedStageCount + 1;

	base.storageMaxCount =
		static_cast<int>(base.storageMaxCount * (1.f + extraLevel * 0.3f));

	base.enemyHP =
		static_cast<int>(base.enemyHP * (1.f + extraLevel * 0.5f));

	base.enemySpeed =
		base.enemySpeed * (1.f + extraLevel * 0.2f);

	base.playerBulletHP =
		static_cast<int>(base.playerBulletHP * (1.f + extraLevel * 0.4f));

	base.bulletScale =
		base.bulletScale * (1.f + extraLevel * 0.15f);

	return base;
}

void InGame::ApplyStageData()
{
	currStageData = GameConfig::Stage::Datas[currentStageIndex];

	for (auto& storage : storages)
	{
		if (storage == nullptr)
			continue;

		storage->SetMaxItemCount(currStageData.storageMaxCount);
	}

	std::cout << "Stage : " << currentStageIndex << std::endl;
}

#pragma endregion