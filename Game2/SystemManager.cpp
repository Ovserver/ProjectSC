#include "stdafx.h"
#include "SystemManager.h"
#include "Unit.h"

void SystemManager::UpdateTileCol()
{
	TileMap->SetTileCol(TileCol::NONE);
	for (size_t i = 0; i < UnitPool.size(); i++)
	{
		TileMap->SetTileCol(UnitPool[i]->GetWorldPos(), TileCol::UNIT);
	}
}
