#pragma once
#include <Actor/Actor.h>
#include <Actor/Player.h>
class ItemCountUI : public Monkey::Actor
{
	TYPE_DECLARATIONS(ItemCountUI, Actor)

public:
	void SetCount(int count)
	{
		itemCount = count;
		std::cout << "----------------------ItemCount : " << itemCount << "\n";
	}

	int GetCount() { return itemCount; }

private:
	int itemCount = 0;
};
