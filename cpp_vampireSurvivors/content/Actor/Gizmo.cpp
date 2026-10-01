#include "Gizmo.h"
#include <Render/RenderTypes.h>

GridActor::GridActor() : Actor()
{
	layer = Layer::Grid;

	SetGrid(Monkey::Color::Red, Monkey::Color::Green, Monkey::Color::Blue, Monkey::Color::White);
}