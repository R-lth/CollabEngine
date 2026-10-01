#pragma once
#include <Render/RenderTypes.h> // Monkey::Color 쓰는 헤더

namespace GameConfig
{
	namespace Stage
	{
		struct StageData
		{
			int storageMaxCount;

			bool canShoot;

			int enemyHP;
			float enemySpeed;
			float enemyScale;
			Monkey::Color enemyColor;

			int playerBulletHP;
			float bulletScale;
			Monkey::Color bulletColor;

			bool addNewStorage;
		};

		static const StageData Datas[] =
		{
			// storageMaxCount; canShoot; enemyHP; enemySpeed; enemyScale
			// ; enemyColor; playerBulletHP; bulletScale; bulletColor;  addNewStorage;
		// 저장고 3개, 총 없음, 적 없음
		{ 3 ,  false/*false */ ,	1,		0.05f,	 2.f,
		Monkey::Color::Black,		1,		0.2f, Monkey::Color::Green, false },

		// 총 해금, 약한 적
		{ 2, true,				 1,		 0.05f,		 2.1f,
		Monkey::Color(0.1f,0.1f,0.1f), 		1, 0.2f, Monkey::Color(0,1.f,0),false},

		// 적 HP 증가
		{ 3, true,				2,		 0.06f,			2.2f,
		Monkey::Color::Black, 	1, 0.25f, Monkey::Color::Blue,false },

		// 총 강화, 적 속도 증가
		{ 4, true,				 2		, 0.7f,		 2.3f,
		Monkey::Color::Black, 	3, 0.35f, Monkey::Color::Red,false },
		};

		constexpr int FixedStageCount = 4;
	}

	namespace Player
	{
		constexpr	int	HP = 100;
		constexpr float	Speed = .05f;
		constexpr float	DetectRange = 100.f;

		constexpr	int BulletHP = 2;

#pragma region followItem

		constexpr float	followOffsetX = 1.f/*0.f*/;
		constexpr float	followOffsetY = .5f;
		constexpr float	followOffsetZ = 1.f;

		//constexpr float StackStartHeight = 0.5f; //followOffset
		constexpr float StackSpacing = 0.5f;

#pragma endregion
	}

	namespace Enemy
	{
		constexpr	int	HP = 4;
		constexpr float	Speed = 0.05f;
		constexpr float	DetectRange = 15.f;
		//constexpr float AttackRange = 1.f;

		constexpr	int BulletHP = 1;

#pragma region - Spawn
		constexpr	int	Count = 2/*4*/;
		constexpr	int	addCount = 0/*4*/;
		constexpr float	StartRadius = 5.f;
		constexpr float	OffsetRadius = 10.f;

		constexpr float	ReSpwanRadius = 10.f /*10.f*/;

#pragma endregion
		constexpr float	DropScale = .3f;

#pragma region Hit
		constexpr float hitScalePower = 1.5f;
		constexpr float knockbackPower = 0.6f;
#pragma endregion
	}

	namespace Item
	{
		constexpr	int HP = 3/*3*/;
		constexpr float Speed = 1.f;
		constexpr float DetectRange = 6.f;
		//constexpr float AttackRange = 1.f;

#pragma region - Spawn
		constexpr	int Count = 7/*20*/;
		constexpr float StartRadius = 3.f;
		constexpr float OffsetRadius = 7.f;
#pragma endregion

#pragma region topanim
		constexpr float BounceSpeed = 5.f;
		constexpr float BounceHeight = .8f;
#pragma endregion
	}

	namespace Bullet
	{
		constexpr	int HP = 2;
		constexpr float Speed = 1.f;
		constexpr float LifeTime = 5.f;
		constexpr float SpawnOffset = 0.07f;

		constexpr float hitTimer = 0.f;
		constexpr float hitDuration = 0.3f;
	}

	namespace Storage
	{
#pragma region - Spawn
		constexpr	int Count = 1/*20*/;
		constexpr float StartRadius = 3.f;
		constexpr float OffsetRadius = 5.f;
#pragma endregion

		constexpr float StackStartHeight = 0.5f;
		constexpr float StackSpacing = .5f;

#pragma region topanim
		constexpr float BounceSpeed = 5.f;
		constexpr float BounceHeight = .8f;
#pragma endregion
	}
}