#pragma once
#include <Engine/Engine.h>
#include <Level/Level.h>

#include <memory>
#include <unordered_map>

class VampireSurvivors : public Monkey::Engine
{
public:
	struct GameState
	{
		int itemCount = 0;
		int storageCount = 0;
		bool weaponUnlock = false;
	};

public:
	VampireSurvivors();
	~VampireSurvivors();

	VampireSurvivors(const VampireSurvivors&) = delete;
	VampireSurvivors operator=(const VampireSurvivors&) = delete;
	VampireSurvivors(VampireSurvivors&&) = delete;
	VampireSurvivors operator=(VampireSurvivors&&) = delete;

	static VampireSurvivors& Get();

public:
	void ToggleLevel();

	void SaveGameState(const GameState& gameState) { this->gameState = gameState; };
	const GameState LoadGameState() const { return gameState; };


private:
	static VampireSurvivors* instance;
	std::unordered_map<Monkey::Engine::State, std::shared_ptr<Monkey::Level>> levels;
	GameState gameState;
};

