#include "stdafx.h"
#include "Utility2.h"
#include "SystemManager.h"
#include "Unit.h"

SystemManager::SystemManager()
{
	TileMap = nullptr;
	UICam = nullptr;

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
	TestBox.SetWorldPos(cmdIconsPos[0]);
}

void SystemManager::Update()
{
	Vector2 temp = INPUT->GetScreenMousePos();
	temp.x -= app.GetHalfWidth(); temp.y -= app.GetHalfHeight(); temp.y *= -1;
	if (!SelectMode)
	{
		if (INPUT->KeyUp(VK_LBUTTON))
		{
			for (size_t i = 0; i < NUM_CMDICON; i++)
			{
				if (cmdIcons[i] && cmdIcons[i]->Intersect(temp))
				{
					if (IconPoolTemp->iconList[i] == CmdIconList::STOP)
						CmdExecute(CmdIconList::STOP);
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
}

void SystemManager::Render()
{
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		UnitPool[i]->col.color = Color(1, 1, 1);
	}
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		UnitPool[i]->Render();
	}
	for (size_t i = 0; i < UnitPoolSelect.size(); i++)
	{
		UnitPoolSelect[i]->col.color = Color(0, 1, 0);
	}
	for (size_t i = 0; i < UnitPoolSelect.size(); i++)
	{
		UnitPoolSelect[i]->col.Render();
	}
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
	SelectMode = false;
	IconPoolTemp = nullptr;
	for (size_t i = 0; i < cmdIcons.size(); i++)
	{
		cmdIcons[i] = nullptr;
	}
	if (!UnitPoolSelect.empty())
	{
		IconPoolTemp = &UnitPool[0]->IconPool;
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
			UnitPoolSelect[i]->Attack();
			break;
		case CmdIconList::PATROL:
			break;
		case CmdIconList::HOLD:
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
			UnitPoolSelect[i]->Attack();
			break;
		case CmdIconList::PATROL:
			break;
		case CmdIconList::HOLD:
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
