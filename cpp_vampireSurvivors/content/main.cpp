#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#include "Game/VampireSurvivors.h"

using namespace Monkey;

int main() 
{
	//::ShowWindow(::GetConsoleWindow(), SW_HIDE); // 콘솔 창 숨김 todo. 디버깅 시에는 켜기

	VampireSurvivors vampireSurvivors;
	vampireSurvivors.Run();
}