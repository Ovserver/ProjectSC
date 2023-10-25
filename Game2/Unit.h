#pragma once
enum class UnitType
{
	UNUSED, UNIT, NEUTRAL, RESOURCE, EFFECT
};
enum class UnitCmd
{
	NONE, STOP, MOVE, ATTACK, PATROL, HOLD, GATHER
};
enum class UnitState
{
	IDLE, MOVE, ATTACK, GATHER
};
class Unit
{
public:
	Unit();
	void Update();
	void Render();
	int  lookDir(Vector2 dir);
	Vector2 GetWorldPos() { return col.GetWorldPos(); }
	void SetWorldPos(Vector2 pos) { col.SetWorldPos(pos); }
	Vector2 GetFoot() { return col.GetWorldPos() + Vector2(0, 10) + keyDir * 10.0f; }
	void InitPath(vector<Tile*> way);
	void GetHealth(int& _hp, int& _maxHp) { _hp = hp; _maxHp = maxHp; }
	void Damage(UINT value) { hp -= value; if (hp <= 0) { hp = 0; Death(); } };
	void Death();

	void Move(Vector2 CommandPos);
	void Attack(Vector2 CommandPos);
	void Hold();
	void Stop();
public:
	bool			pathfinding = false;
	float			attackRange;
	float			sightRange;
	int				atkDamage;
	UINT			playerNum;
	ObRect			col;
	Vector2			cmdPos;
	Vector2			originPos;
	vector<Tile*>	pathWay;
	Unit*			targetCmdUnit;
	CmdIconPool		IconPool;
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
	float			deathTime;
	int				hp;
	int				maxHp;
	float			tickAttackCooldown = 0;
	const float		attackCooldown = 1.0f;
	float			tickPathUpdateTime = 0;
	const float		PathUpdateTime = 5.0f;
	UnitCmd			unitCmd;
};
