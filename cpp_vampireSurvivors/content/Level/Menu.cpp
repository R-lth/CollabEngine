#include "Menu.h"
#include <Core/Input.h>
#include <Render/Renderer.h>
#include <string>

#include "Game/VampireSurvivors.h"

Menu::Menu() : Level()
{
}

void Menu::OnInitialized()
{
	Level::OnInitialized();

	Monkey::Renderer::Get().uiText[0] = L"플레이어 재화: 0";
	Monkey::Renderer::Get().uiText[1] = L"저장소 재화: 0";
	Monkey::Renderer::Get().uiText[2] = L"총 무기 해금 (필요 재화 3개)";
}

void Menu::Update(float deltaTime)
{
	Level::Update(deltaTime);

	if (bEnter) 
	{
		LoadPlayerInfo();
		bEnter = false;
	}

	if (Monkey::Input::Get().GetKeyDown('M'))
	{
		bEnter = true;
		VampireSurvivors::Get().ToggleLevel();
	}

	if (Monkey::Input::Get().GetKeyDown(VK_ESCAPE))
	{
		VampireSurvivors::Get().Quit();
	}

	POINT mousePos = Monkey::Input::Get().GetMousePosition();

	const D2D1_RECT_F rect = Monkey::Renderer::Get().GetGraphics().GetMenuUIRect()[2];
	const bool isHover = mousePos.x >= rect.left && mousePos.x <= rect.right && mousePos.y >= rect.top/*윈도우 좌표계*/ && mousePos.y <= rect.bottom;
	if (isHover && (Monkey::Input::Get().GetMouseButtonDown(Monkey::Input::MouseButton::Left)))
	{
		if (!gameData.weaponUnlock && gameData.storageCount >= 3)
		{
			UnlockWeapon();
		}
	}
}

void Menu::LoadPlayerInfo()
{
	const VampireSurvivors::GameState state =
		VampireSurvivors::Get().LoadGameState();

	if (state.itemCount == gameData.itemCount &&
		state.storageCount == gameData.storageCount &&
		state.weaponUnlock == gameData.weaponUnlock)
	{
		return;
	}

	gameData.itemCount = state.itemCount;
	gameData.storageCount = state.storageCount;

	Monkey::Renderer::Get().uiText[0] = L"플레이어 재화: " + std::to_wstring(gameData.itemCount);
	Monkey::Renderer::Get().uiText[1] = L"저장소 재화: " + std::to_wstring(gameData.storageCount);
}

void Menu::UnlockWeapon()
{
	gameData.storageCount -= 3;
	gameData.weaponUnlock = true;

	Monkey::Renderer::Get().uiText[1] = L"저장소 재화: " + std::to_wstring(gameData.storageCount);
	Monkey::Renderer::Get().uiText[2] = L"샷건 해금 완료";

	VampireSurvivors::GameState state = VampireSurvivors::Get().LoadGameState();
	state.storageCount = gameData.storageCount;
	state.weaponUnlock = gameData.weaponUnlock;
	VampireSurvivors::Get().SaveGameState(state);
}
