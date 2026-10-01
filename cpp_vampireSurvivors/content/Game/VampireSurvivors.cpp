#include "vampireSurvivors.h"
#include "Level/InGame.h"
#include "Level/Menu.h"

#include <cassert>

VampireSurvivors* VampireSurvivors::instance = nullptr;

VampireSurvivors::VampireSurvivors() : Engine()
{
	assert(!instance);
	instance = this;

	levels[Monkey::Engine::State::Game3D] = std::make_shared<InGame>();
	levels[Monkey::Engine::State::Menu] = std::make_shared<Menu>();

	mainLevel = levels[Monkey::Engine::State::Game3D];
}

VampireSurvivors::~VampireSurvivors()
{
	instance = nullptr;
}

VampireSurvivors& VampireSurvivors::Get()
{
	assert(instance);
	return *instance;
}

void VampireSurvivors::ToggleLevel()
{
	if (state == Monkey::Engine::State::Game3D)
	{
		state = Monkey::Engine::State::Menu;
	}
	else
	{
		state = Engine::State::Game3D;
	}

	mainLevel = levels[state];
}

