#pragma once
class Unit;
class SystemManager : public Singleton<SystemManager>
{
public:
	ObTileMap*		TileMap;
public:
	vector<Unit*>	UnitPool;
	vector<Unit*>	UnitPoolSelect;
public:
	void UpdateTileCol();
};
