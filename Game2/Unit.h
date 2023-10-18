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

	void Move(Vector2 CommandPos);
	void Attack();
	void Stop();
public:
	bool			pathfinding = false;
	ObRect			col;
	vector<Tile*>	pathWay;
	Vector2			cmdPos;
protected:
	int				dirFrameY[8];
	float			moveSpeed;

	Vector2         keyDir;
	Vector2			moveDir;

	UnitState		unitState;
	UnitType		unitType;

	ObImage			spriteIdle;
	ObImage			spriteMove;
	ObImage			spriteAttack;
private:
	float			tickPathUpdateTime = 0;
	const float		PathUpdateTime = 0.25f;
};

