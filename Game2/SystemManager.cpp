#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"

SystemManager::SystemManager()
{
	for (size_t i = 0; i < NUM_CMDICON; i++)
		cmdIcons.push_back(nullptr);

	Utility2::InitImage(IMOVE, L"icons/cmdiconsMove.png");
	Utility2::InitImage(ISTOP, L"icons/cmdiconsStop.png");
	Utility2::InitImage(IATTACK, L"icons/cmdiconsAttack.png");
	Utility2::InitImage(IPATROL, L"icons/cmdiconsPatrol.png");
	Utility2::InitImage(IHOLD, L"icons/cmdiconsHold.png");

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Vector2 temp;
			temp.x = -app.GetHalfWidth() + (1048 + j * 92);
			temp.y = app.GetHalfHeight() - (750 + i * 80);
			cmdIconsPos.push_back(temp);
		}
	}
}

void SystemManager::Update()
{
	if (IMOVE.Intersect(INPUT->GetWorldMousePos()))
		cout << "move icons" << endl;
}

void SystemManager::Render()
{
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		if (cmdIcons[i])
			cmdIcons[i]->Render(UICam);
	}
}

void SystemManager::UpdateTileCol()
{
	TileMap->SetTileCol(TileCol::NONE);
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		TileMap->SetTileCol(UnitPool[i]->GetWorldPos(), TileCol::UNIT);
	}
}

void SystemManager::UpdateCmdIcons()
{
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		cmdIcons[i] = nullptr;
	}
	if (!UnitPoolSelect.empty())
	{
		cmdIcons[0] = &IMOVE; cmdIcons[0]->SetWorldPos(cmdIconsPos[0]);
		cmdIcons[1] = &ISTOP; cmdIcons[1]->SetWorldPos(cmdIconsPos[1]);
		cmdIcons[2] = &IATTACK; cmdIcons[2]->SetWorldPos(cmdIconsPos[2]);
		cmdIcons[3] = &IPATROL; cmdIcons[3]->SetWorldPos(cmdIconsPos[3]);
		cmdIcons[4] = &IHOLD; cmdIcons[4]->SetWorldPos(cmdIconsPos[4]);
	}
}
