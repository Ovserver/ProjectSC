#include "stdafx.h"
#include "Utility2.h"

bool Utility2::ShowCollider = true;

void Utility2::InitImage(ObImage& image, wstring fName, Vector2 offset, int xFrame, int yFrame, bool isRender)
{
	image.LoadFile(fName);
	image.SetPivot() = offset;
	image.maxFrame.x = xFrame;
	image.maxFrame.y = yFrame;
	image.SetScale().x = image.imageSize.x / xFrame * IMGSCALE;
	image.SetScale().y = image.imageSize.y / yFrame * IMGSCALE;
	image.isVisible = isRender;
}

bool DynamicMapWalkable(ObTileMap* gameMap, int x, int y)
{
	for (size_t n = 0; n < 4; n++)
	{
		for (size_t m = 0; m < 4; m++)
		{
			if (!gameMap->walkableTiles[x * 4 + n][y * 4 + m])
				return false;
		}
	}
	return true;
}

void Utility2::InitDynamicMap(ObTileMap* GameMap, ObTileMap* dynamicMap)
{
	for (size_t i = 0; i < dynamicMap->tileSize.x; i++)
	{
		dynamicMap->walkableTiles.push_back(vector<bool>());
		for (size_t j = 0; j < dynamicMap->tileSize.y; j++)
		{
			dynamicMap->walkableTiles[i].push_back(true);
			dynamicMap->walkableTiles[i][j] = DynamicMapWalkable(GameMap, i, j);
		}
	}
}

