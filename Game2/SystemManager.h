#pragma once
#define NUM_CMDICON 9

enum class CmdIconList
{
	MOVE, STOP, ATTACK, PATROL, HOLD, GATHER, CANCEL, NONE
};
struct CmdIconPool
{
	CmdIconList iconList[9];
	CmdIconPool() {
		for (size_t i = 0; i < NUM_CMDICON; i++)
		{
			iconList[i] = CmdIconList::NONE;
		}
	}
};
class Unit;
class SystemManager : public Singleton<SystemManager>
{
public:
	SystemManager();
	void Update();
	void Render();
	void UpdateTileCol();
	void UpdateCmdIcons();
	void CmdPosSelectMod();
	void CmdExecute();
	void CmdExecute(CmdIconList command);
	void DeleteUnitPool();
	bool CreateBuilding(UnitType unitType, UnitName unitName, ObRect* gridArray, UINT gridX, UINT gridY);
public:
	bool	SelectMode;
	Camera* UICam;
	ObImage	IMOVE;
	ObImage	IATTACK;
	ObImage	ISTOP;
	ObImage	IHOLD;
	ObImage	IPATROL;
	ObImage	ICANCEL;
	ObImage	TestBox;

	Vector2			cmdPos;
	UINT			cmdIconsNum;
	vector<ObImage*> cmdIcons;
	vector<Vector2> cmdIconsPos;
	vector<ObImage*>unitWireframe;
	vector<Vector2> unitWireframePos;
	ObTileMap*		GameMap;
	ObTileMap*		DynamicGameMap;
	vector<Unit*>	UnitPool;
	vector<Unit*>	UnitPoolSelect;
	vector<Unit*>	UnitPoolDelete;
	vector<Unit*>	BuildingPool;
	Unit*			BuildingPoolSelect;

	CmdIconPool*	IconPoolTemp;
	CmdIconPool*	IconPoolTempSelect;
	CmdIconPool		IconPoolSelect;
private:
	ObImage console;
};
