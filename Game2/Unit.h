#pragma once

class Unit
{
public:
	static void InitWireframes();
	Unit(UnitType _unitType = UnitType::UNIT, UnitName _unitName = UnitName::ZEALOT);
	~Unit();
	void Release();
	void Update();
	void Render();
	int  lookDir(Vector2 dir);
	Vector2 GetWorldPos() { return col.GetWorldPos(); }
	void SetWorldPos(Vector2 pos) { col.SetWorldPos(pos); }
	Vector2 GetFoot() { return col.GetWorldPos() + Vector2(0, 10) + keyDir * 10.0f; }
	void InitPath(vector<Tile*> way);
	void InitPath2(vector<INTPAIR> way);
	void GetHealth(int& _hp, int& _maxHp) { _hp = hp; _maxHp = maxHp; }
	void Damage(UINT value) { hp -= value; if (hp <= 0) { hp = 0; Death(); } };
	void Death();

	void Move(Vector2 CommandPos);
	void Attack(Vector2 CommandPos);
	void Hold();
	void Stop();
	void Stop2();
public:
	static ObTileMap* GameMap;
	static map<UnitName, ObImage*> UnitWireframe;
	bool			pathfinding = false;
	bool			stuck = false;
	float			attackRange;
	float			sightRange;
	int				atkDamage;
	float			stuckTime = 0;
	UINT			playerNum;
	ObRect			col;
	ObCircle		attackRangeCol;
	ObCircle		sightRangeCol;
	Vector2			cmdPos;
	Vector2			originPos;
	vector<Tile*>	pathWay;
	vector<INTPAIR>	pathWay2;
	Unit*			targetCmdUnit;
	Unit*			stuckedUnit;
	CmdIconPool		IconPool;
	UnitName		unitName;
	vector<Int2>	buildingColGrid;
protected:
	int				dirFrameX[9];
	float			moveSpeed;
	bool			flipImage;
	Vector2         keyDir;
	Vector2			moveDir;

	UnitState		unitState;
	UnitType		unitType;

	ObImage			spriteIdle;
	ObImage			spriteMove;
	ObImage			spriteAttack;
	ObImage			spriteDeath;
private:
	void InitUnitImage();
private:
	float			deathTime;
	int				hp;
	int				maxHp;
	float			tickAttackCooldown = 0;
	const float		attackCooldown = 1.0f;
	float			tickPathUpdateTime = 0;
	const float		PathUpdateTime = 100.5f;
	UnitCmd			unitCmd;
};
