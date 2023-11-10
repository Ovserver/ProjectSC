#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"

SystemManager::SystemManager()
{
	GameMap = nullptr;
	DynamicGameMap = nullptr;
	UICam = nullptr;

	Utility2::InitImage(console, L"console/tconsole.png");

	Utility2::InitImage(IMOVE, L"icons/cmdiconsMove.png"); IMOVE.collider = COLLIDER::RECT;
	Utility2::InitImage(ISTOP, L"icons/cmdiconsStop.png"); ISTOP.collider = COLLIDER::RECT;
	Utility2::InitImage(IATTACK, L"icons/cmdiconsAttack.png"); IATTACK.collider = COLLIDER::RECT;
	Utility2::InitImage(IPATROL, L"icons/cmdiconsPatrol.png"); IPATROL.collider = COLLIDER::RECT;
	Utility2::InitImage(IHOLD, L"icons/cmdiconsHold.png"); IHOLD.collider = COLLIDER::RECT;
	Utility2::InitImage(ICANCEL, L"icons/cmdiconsCancel.png"); ICANCEL.collider = COLLIDER::RECT;

	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			Vector2 temp;
			temp.x = -app.GetHalfWidth() + (1048 + j * 92);
			temp.y = app.GetHalfHeight() - (750 + i * 80);
			cmdIconsPos.push_back(temp);
			cmdIcons.push_back(nullptr);
		}
	}
	IconPoolTemp = nullptr;
	IconPoolTempSelect = nullptr;
	for (size_t i = 0; i < NUM_CMDICON; i++)
	{
		if (i != NUM_CMDICON - 1)
			IconPoolSelect.iconList[i] = CmdIconList::NONE;
		else
			IconPoolSelect.iconList[i] = CmdIconList::CANCEL;
	}
	SelectMode = false;
	Utility2::InitImage(TestBox, L"Tile2.png");
	for (size_t i = 0; i < 6; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			unitWireframePos.push_back(Vector2(
				-app.GetHalfWidth() + (380 + i * 75),
				app.GetHalfHeight() - (828 + j * 87)));
			unitWireframe.push_back(nullptr);
		}
	}
}

void SystemManager::Update()
{
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		UnitPool[i]->Update();
	}
	Vector2 temp = INPUT->GetScreenMousePos();
	temp.x -= app.GetHalfWidth(); temp.y -= app.GetHalfHeight(); temp.y *= -1;
	if (!SelectMode)
	{
		if(INPUT->KeyDown('S'))
		{
			CmdExecute(CmdIconList::STOP);
		}
		if(INPUT->KeyDown('H'))
		{
			CmdExecute(CmdIconList::HOLD);
		}
		if(INPUT->KeyDown('A'))
		{
			cmdIconsNum = 2;
			CmdPosSelectMod();
		}
		if (INPUT->KeyUp(VK_LBUTTON))
		{
			for (size_t i = 0; i < NUM_CMDICON; i++)
			{
				if (cmdIcons[i] && cmdIcons[i]->Intersect(temp))
				{
					if (IconPoolTemp->iconList[i] == CmdIconList::STOP)
						CmdExecute(CmdIconList::STOP);
					else if (IconPoolTemp->iconList[i] == CmdIconList::HOLD)
						CmdExecute(CmdIconList::HOLD);
					else
					{
						cmdIconsNum = i;
						CmdPosSelectMod();
						break;
					}
				}
			}
		}
	}
	else
	{
		if (INPUT->KeyDown(VK_LBUTTON)) {
			cmdPos = INPUT->GetWorldMousePos();
			CmdExecute();
		}
	}
	if (!UnitPoolDelete.empty())
	{
		DeleteUnitPool();
	}

}

void SystemManager::Render()
{
	
	for (size_t i = 0; i < BuildingPool.size(); i++)
	{
		BuildingPool[i]->Render();
	}
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		UnitPool[i]->col.color = Color(1, 1, 1);
		if (UnitPool[i]->unitType != UnitType::AIRUNIT)
			UnitPool[i]->Render();
	}
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		if (UnitPool[i]->unitType == UnitType::AIRUNIT)
			UnitPool[i]->Render();
	}
	for (size_t i = 0; i < UnitPoolSelect.size(); i++)
	{
		UnitPoolSelect[i]->col.color = Color(0, 1, 0);
		UnitPoolSelect[i]->col.Render();
	}
	console.Render(UICam);
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		if (cmdIcons[i])
			cmdIcons[i]->Render(UICam);
	}
	if (UnitPoolSelect.size() > 0)
	{
		for (size_t i = 0; i < unitWireframe.size(); i++)
		{
			if (unitWireframe[i])
			{
				unitWireframe[i]->SetWorldPos(unitWireframePos[i]);
				unitWireframe[i]->Render(UICam);
			}
		}
	}
}

void SystemManager::UpdateTileCol()
{
	GameMap->SetTileCol(TileCol::NONE);
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		GameMap->SetTileCol(UnitPool[i]->GetWorldPos(), TileCol::GROUNDUNIT);
	}
}

void SystemManager::UpdateCmdIcons()
{
	SelectMode = false;
	IconPoolTemp = nullptr;
	for (size_t i = 0; i < unitWireframe.size(); i++)
	{
		unitWireframe[i] = nullptr;
	}
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		cmdIcons[i] = nullptr;
	}
	if (!UnitPoolSelect.empty())
	{
		for (size_t i = 0; i < UnitPoolSelect.size(); i++)
		{
			unitWireframe[i] = Unit::UnitWireframe[UnitPoolSelect[i]->unitName];
		}
		IconPoolTemp = &UnitPoolSelect.front()->IconPool;
		for (size_t i = 0; i < NUM_CMDICON; i++)
		{
			switch (IconPoolTemp->iconList[i])
			{
			case CmdIconList::MOVE:
				cmdIcons[i] = &IMOVE; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
				break;
			case CmdIconList::STOP:
				cmdIcons[i] = &ISTOP; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
				break;
			case CmdIconList::ATTACK:
				cmdIcons[i] = &IATTACK; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
				break;
			case CmdIconList::PATROL:
				cmdIcons[i] = &IPATROL; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
				break;
			case CmdIconList::HOLD:
				cmdIcons[i] = &IHOLD; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
				break;
			case CmdIconList::GATHER:
				break;
			case CmdIconList::CANCEL:
				cmdIcons[i] = &ICANCEL; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
				break;
			case CmdIconList::NONE:
				break;
			default:
				break;
			}
		}
	}
}

void SystemManager::CmdPosSelectMod()
{
	IconPoolTempSelect = IconPoolTemp;
	IconPoolTemp = &IconPoolSelect;
	SelectMode = true;
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		cmdIcons[i] = nullptr;
	}
	for (size_t i = 0; i < NUM_CMDICON; i++)
	{
		switch (IconPoolTemp->iconList[i])
		{
		case CmdIconList::MOVE:
			cmdIcons[i] = &IMOVE; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::STOP:
			cmdIcons[i] = &ISTOP; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::ATTACK:
			cmdIcons[i] = &IATTACK; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::PATROL:
			cmdIcons[i] = &IPATROL; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::HOLD:
			cmdIcons[i] = &IHOLD; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::GATHER:
			break;
		case CmdIconList::CANCEL:
			cmdIcons[i] = &ICANCEL; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::NONE:
			break;
		default:
			break;
		}
	}
}
void SystemManager::CmdExecute()
{
	for (size_t i = 0; i < UnitPoolSelect.size(); i++)
	{
		switch (IconPoolTempSelect->iconList[cmdIconsNum])
		{
		case CmdIconList::MOVE:
			UnitPoolSelect[i]->Move(cmdPos);
			break;
		case CmdIconList::STOP:
			UnitPoolSelect[i]->Stop();
			break;
		case CmdIconList::ATTACK:
			UnitPoolSelect[i]->Attack(cmdPos);
			break;
		case CmdIconList::PATROL:
			break;
		case CmdIconList::HOLD:
			UnitPoolSelect[i]->Hold();
			break;
		case CmdIconList::GATHER:
			break;
		case CmdIconList::CANCEL:
			break;
		case CmdIconList::NONE:
			break;
		default:
			break;
		}
	}
	IconPoolTemp = IconPoolTempSelect;
	SelectMode = false;
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		cmdIcons[i] = nullptr;
	}
	for (size_t i = 0; i < NUM_CMDICON; i++)
	{
		switch (IconPoolTemp->iconList[i])
		{
		case CmdIconList::MOVE:
			cmdIcons[i] = &IMOVE; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::STOP:
			cmdIcons[i] = &ISTOP; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::ATTACK:
			cmdIcons[i] = &IATTACK; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::PATROL:
			cmdIcons[i] = &IPATROL; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::HOLD:
			cmdIcons[i] = &IHOLD; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::GATHER:
			break;
		case CmdIconList::CANCEL:
			cmdIcons[i] = &ICANCEL; cmdIcons[i]->SetWorldPos(cmdIconsPos[i]);
			break;
		case CmdIconList::NONE:
			break;
		default:
			break;
		}
	}
}

void SystemManager::CmdExecute(CmdIconList command)
{
	for (size_t i = 0; i < UnitPoolSelect.size(); i++)
	{
		switch (command)
		{
		case CmdIconList::MOVE:
			UnitPoolSelect[i]->Move(cmdPos);
			break;
		case CmdIconList::STOP:
			UnitPoolSelect[i]->Stop();
			break;
		case CmdIconList::ATTACK:
			UnitPoolSelect[i]->Attack(cmdPos);
			break;
		case CmdIconList::PATROL:
			break;
		case CmdIconList::HOLD:
			UnitPoolSelect[i]->Hold();
			break;
		case CmdIconList::GATHER:
			break;
		case CmdIconList::CANCEL:
			break;
		case CmdIconList::NONE:
			break;
		default:
			break;
		}
	}
}

void SystemManager::DeleteUnitPool()
{
	bool flag = false;
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		if (UnitPool[i] == UnitPoolDelete.back())
		{
			UnitPool.erase(UnitPool.begin() + i);
			//UnitPool.shrink_to_fit();
			flag = true;
			break;
		}
	}
	for (size_t i = 0; i < UnitPoolSelect.size(); i++)
	{
		if (UnitPoolSelect[i] == UnitPoolDelete.back())
		{
			UnitPoolSelect.erase(UnitPoolSelect.begin() + i);
			//UnitPoolSelect.shrink_to_fit();

			flag = true;
			break;
		}
	}
	if (flag)
	{
		UnitPoolDelete.back()->Release();
		delete UnitPoolDelete.back();
		UnitPoolDelete.pop_back();
	}
}

bool SystemManager::CreateBuilding(UnitType unitType, UnitName unitName, ObRect* gridArray, UINT gridX, UINT gridY)
{
	for (size_t i = 0; i < 12; i++)
	{
		if (!DynamicGameMap->GetTileWalkable(gridArray[i].GetWorldPos()))
		{
			return false;
		}
	}
	// create building
	Int2 int2tmp;
	DynamicGameMap->WorldPosToTileIdx(INPUT->GetWorldMousePos(), int2tmp);
	Unit* temp = new Unit(unitType, unitName);
	temp->SetWorldPos(DynamicGameMap->Tiles[int2tmp.x][int2tmp.y].Pos - Vector2(32, 0));
	for (size_t i = 0; i < gridX * gridY; i++)
	{
		DynamicGameMap->WorldPosToTileIdx(gridArray[i].GetWorldPos(), int2tmp);
		DynamicGameMap->walkableTiles[int2tmp.x][int2tmp.y] = false;
		temp->buildingColGrid.push_back(int2tmp);
	}
	GameMap->UpdateBuildingTiles(DynamicGameMap, temp->buildingColGrid, true);
	UnitPool.push_back(temp);
	return true;
}
