#pragma once
#define NUM_CMDICON 9
class Unit;
class SystemManager : public Singleton<SystemManager>
{
public:
	Camera*	UICam;
	ObImage	IMOVE;
	ObImage	IATTACK;
	ObImage	ISTOP;
	ObImage	IHOLD;
	ObImage	IPATROL;

	vector<ObImage*> cmdIcons;
	vector<Vector2> cmdIconsPos;
	ObTileMap*		TileMap;
	vector<Unit*>	UnitPool;
	vector<Unit*>	UnitPoolSelect;
public:
	SystemManager();
	void Update();
	void Render();
	void UpdateTileCol();
	void UpdateCmdIcons();
};
