#pragma once
#include <Level/Level.h>
#include <ctime>
#include "../Actor/Gizmo.h"

#include "../Actor/Player.h"
#include "../Actor/Enemy.h"
#include "../Actor/Item.h"
#include "../Actor/Bullet.h"
#include "../Actor/ItemCountUI.h"
#include "../Actor/Storage.h"

#include "../Actor/GameConfig.h"
#include <cfloat>
//#include <main.cpp>

enum TESTT
{
	TEST,

	none
};

class InGame : public Monkey::Level
{
	TESTT ttype = TESTT::none;

public:
	InGame();

private:
	virtual void OnInitialized() override;
	virtual void Update(float deltaTime) override;
	void SaveGameInfo() const;

public:
	std::shared_ptr<Player> player;
	std::vector<std::shared_ptr<Enemy>> enemies;
	std::vector<std::shared_ptr<Item>> items;
	std::vector<std::shared_ptr<Storage>> storages;

#pragma region Init / Spawn

private:
	/// <summary>
	/// 액터 생성
	/// </summary>
	/// <typeparam name="T">액터타입</typeparam>
	/// <param name="count">생성할 적 수</param>
	/// <param name="start"> 생성 시작 거리</param>
	/// <param name="offset"> 생성 간격 거리</param>
	template<typename T>
	void SpawnMoveableActor(int count, float start, float offset);

	/// <summary>
	/// 액터 생성 후 담기
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="actorList"> 담을 객체 </param>
	/// <param name="count">생성할 적 수</param>
	/// <param name="start"> 생성 시작 거리</param>
	/// <param name="offset"> 생성 간격 거리</param>
	template<typename T>
	void SpawnMoveableActor(std::vector<std::shared_ptr<T>>& actorList, int count, float start, float offset);

	/// <summary>
	/// 적 생성
	/// </summary>
	/// <param name="count">생성할 적 수</param>
	/// <param name="start"> 생성 시작 거리</param>
	/// <param name="offset"> 생성 간격 거리</param>
	void SpawnEnemy(int count, float start, float offset);

	/// <summary>
	/// 도넛형태 랜덤 위치 가져오기
	/// </summary>
	/// <param name="x">생성중심좌표</param>
	/// <param name="z">생성중심좌표</param>
	/// <param name="start">생성 시작 거리</param>
	/// <param name="offset">생성 간격 거리</param>
	/// <returns></returns>
	Monkey::Vector3	_GetRandomSpawnPoslikeDoughnut(float _x, float _z, float start, float offset);
public:
	void SpawnItem(ItemType itemType, Monkey::Vector3 position, Monkey::Vector3  hitKnockbackDir);

#pragma endregion

#pragma region Attack / Bullet
public:
	std::shared_ptr<Enemy> FindNearestEnemy();
	void PlayerAttack();
	//void CollisionEnemy(const std::shared_ptr<Monkey::Actor>& bullet, const std::shared_ptr<Monkey::Actor>& other);

#pragma endregion

	void RespawnEnemy(const std::shared_ptr<Monkey::Actor>& enemyActor);

public:

	std::shared_ptr<Player> GetPlayer() const { return player; }

public:
	GameConfig::Stage::StageData currStageData;

	int currentStageIndex = 0;

	void SetStageIndex(int index) { currentStageIndex = index; }
	GameConfig::Stage::StageData GetStageData(int stageIndex);
	void ApplyStageData();
	void CheckStageProgress();
	void AdvanceStage();

private:
	bool bEnter = true;
	bool weaponUnlock = false;
};
