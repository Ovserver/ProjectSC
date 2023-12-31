#pragma once
enum DirState
{
	A0000, A0225, A0450, A0675, A0900, A1125, A1350, A1575, A1800
};
class Utility2
{
public:
	static bool ShowCollider;
public:
	static void InitImage(ObImage& image, wstring fName, Vector2 offset = Vector2(0, 0), int xFrame = 1, int yFrame = 1, bool isRender = true);
	// GameMap의 tile State 정보를 읽고, dynamicMap의 크기에 맞춰 walkable 설정
	static void InitDynamicMap(ObTileMap* GameMap, ObTileMap* dynamicMap);
};

