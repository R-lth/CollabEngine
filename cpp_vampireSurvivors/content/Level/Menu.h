#pragma once
#include <Level/Level.h>

class Menu : public Monkey::Level
{
	struct GameData
	{
		int itemCount = 0;
		int storageCount = 0;
		bool weaponUnlock = false;
	};

public:
	Menu();
	~Menu() = default;

private:
	void OnInitialized() override;
	void Update(float deltaTime) override;
	void LoadPlayerInfo();
	void UnlockWeapon();

private:
	GameData gameData;
	bool bEnter = true;
};

